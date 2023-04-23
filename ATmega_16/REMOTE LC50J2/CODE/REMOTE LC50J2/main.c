#include "StdTypes.h"
#include "LED.h"
#include "USART.h"

#define F_CPU           8000000
#define ICP_FALLING_EDGE      (0 << ICES1)
#define ICP_RISING_EDGE      (1 << ICES1)
#define OFFSET        5

#define DEBUG           0

volatile uint16_t MAX = 0;
volatile uint16_t MIN = 0;
static volatile uint8_t nIdx = 0;
volatile uint16_t startBitsCode[4];

static volatile BOOL bMatchStartCode0 = FALSE;
static volatile BOOL bMatchStartCode1 = FALSE;
static volatile BOOL bMatchStartCode2 = FALSE;

#if (DEBUG == 1)
volatile uint16_t data[64];
volatile uint16_t data_len;
#endif /*#if (DEBUG == 1)*/
static volatile uint32_t keycode;
static volatile uint8_t keycode_idx = 0;
static volatile uint16_t stick;

BOOL compare_equal(uint16_t a, uint16_t b);

void InfraredDecode_Init(void)
{
  TCCR1A = 0x00; /* Normal port operation, OC1A/OC1B disconnected */
  TCCR1B =  ((1 << ICNC1)|  /* Activates the Input Capture Noise Canceler */
            ICP_FALLING_EDGE|  /* Input Capture Edge Select: rising (positive) edge */
            (1 << WGM12)|  /* CTC mode */
            (0 << CS12)|(0 << CS11)|(1 << CS10)); /* PRESCALE = 1 */
  /****** Set interrupt 100us *********/
  /* fo = 12Mhz / 1 = 12000Khz */
  /* ==> OCR1A = 0.1ms * 12000Khz/PRESCALE */
  OCR1A = F_CPU_Khz/10;
  TIMSK |= (1 << TICIE1);  /* Input Capture Interrupt Enable */
}

void TimerStop(void)
{
  LED1_OFF;
  TCNT1 = 0;
  stick = 0;
  nIdx = 0;
  MIN = 0;
  MAX = 0;
  keycode = 0;
#if (DEBUG == 1)
  data_len = 0;
#endif /*#if (DEBUG == 1)*/  
  keycode_idx = 0;
  bMatchStartCode0 = FALSE;
  bMatchStartCode1 = FALSE;
  bMatchStartCode2 = FALSE;
        
  TIFR |= (1 << ICF1) | (1 << OCF1A);
  TIMSK &= ~(1 << OCIE1A);
}

int main(void)
{
  uint16_t tmp1 = 0;
  uint16_t tmp2 = 0;
  uint8_t code1 = 0;
  uint8_t code2 = 0;
  uint8_t code3 = 0;
  uint8_t code4 = 0;
  InfraredDecode_Init();
  LED_Configuration();
  USART_Init();
  stick = 0;
  __enable_interrupt();
  LED1_ON;
  _delay_ms(200);
  LED1_OFF;
  while(1)
  {
    /* Timer run free-time*/
    
    //if ((stick >= 500))
#if (DEBUG == 1)    
    if (FALSE)
#else
    if ((stick >= 500))
#endif /*#if (DEBUG == 1)*/     
    {
      tmp1 = keycode >> 16;
      tmp2 = (uint16_t)keycode;
      code1 = tmp1 >> 8;
      code2 = (uint8_t)tmp1;
      code3 = tmp2 >> 8;
      code4 = (uint8_t)tmp2;
      if ((code2 != 0) || (code3 != 0) || (code4 != 0))
      {
        USART_Transmit(code1);
        USART_Transmit(code2);
        USART_Transmit(code3);
        USART_Transmit(code4);
      }
      TimerStop();
      
    }
  }
}

ISR (USART_RXC_vect)
{ 
  uint8_t dataRev = UDR;
#if (DEBUG == 1) 
  uint16_t tmp1 = 0;
  uint16_t tmp2 = 0;
  uint8_t code1 = 0;
  uint8_t code2 = 0;
  uint8_t code3 = 0;
  uint8_t code4 = 0;
  if (dataRev == 'a')
  {
    LED1_OFF;
    tmp1 = keycode >> 16;
    tmp2 = (uint16_t)keycode;
    code1 = tmp1 >> 8;
    code2 = (uint8_t)tmp1;
    code3 = tmp2 >> 8;
    code4 = (uint8_t)tmp2;
    for(int i = 0; i < data_len; i++)
    {
      USART_Transmit((uint8_t)data[i]);
    }
    USART_Transmit(MAX);
    USART_Transmit(MIN);
    USART_Transmit(code1);
    USART_Transmit(code2);
    USART_Transmit(code3);
    USART_Transmit(code4);
    TimerStop();
    /*Stop timer and clear stick */    
    
  }
#endif /*#if (DEBUG == 1)*/   
}
ISR(TIMER1_CAPT_vect)
{
  uint8_t sreg;
  uint16_t nStick = 0;
  nStick = stick; /* logging stick*/
  /* Save Global Interrupt Flag*/
  sreg = SREG;
  /* Disable interrupts*/
  __disable_interrupt();
  LED1_ON;
  if (nIdx > 0) //Ignore the first ISR
  {
    if (nIdx <= 4) // Start Bit Codes
    {
      /* Save Start Bits Code */
      startBitsCode[nIdx - 1] = nStick;
#if (DEBUG == 1)
      data[data_len++] = nStick;
#endif /*#if (DEBUG == 1)*/      
    }
    else
    {
 #if (DEBUG == 1)   
      data[data_len++] = nStick;
#endif /*#if (DEBUG == 1)*/           
      /* Check if overlap */
      if (compare_equal(nStick, startBitsCode[0]))
      {
        bMatchStartCode0 = TRUE;
      }
      else if ((bMatchStartCode0 == TRUE) && (compare_equal(nStick, startBitsCode[1])))
      {        
        bMatchStartCode0 = FALSE;
        bMatchStartCode1 = TRUE;
      }
      else if ((bMatchStartCode1 == TRUE) && (compare_equal(nStick, startBitsCode[2])))
      {
        bMatchStartCode0 = FALSE;
        bMatchStartCode1 = FALSE;
        bMatchStartCode2 = TRUE;
      }
      else if ((bMatchStartCode2 == TRUE) && (compare_equal(nStick, startBitsCode[3])))
      {
        bMatchStartCode0 = FALSE;
        bMatchStartCode1 = FALSE;
        bMatchStartCode2 = FALSE;
        MIN = 0;
        MAX = 0;
        keycode = 0;
        keycode_idx = 0;
#if (DEBUG == 1)        
        data_len = 0;
#endif /*#if (DEBUG == 1)*/         
      }
      else /* Not overlap*/
      {
        if (MIN == 0)
        {
          if (MAX == 0)
          {
            MAX = nStick;
          }
          else
          {
            if (compare_equal(nStick, MAX) == FALSE)
            {
              if (nStick < MAX)
              {
                MIN = nStick;
                keycode <<= 1;
                keycode |= 1;
                keycode <<= 1;
              }
              else
              {
                MIN = MAX;
                MAX = nStick;
                keycode |= 1;
                keycode <<= 1;
                keycode <<= 1;
              }
              keycode_idx += 2;
            }
          }
        }
        /* we have max, min --> so we calculate KeyCode */
        else if ((MIN > 0) && (MAX > 0) && (keycode_idx < 31))
        {
          if (compare_equal(nStick, MIN))
          {
            keycode <<= 1;
            keycode_idx++;
          }
          else if (compare_equal(nStick, MAX))
          {
            keycode |= 1;
            keycode <<= 1;
            keycode_idx++;
          }
          else
          {
            /* do nothing */
          }
        }
      }
    }
  }
  TCNT1 = 0;
  stick = 0;
  TIMSK |= (1 << OCIE1A);   /* Output Compare A Match Interrupt Enable */
  nIdx++;       //Count number of bits
  /* Restore Global Interrupt Flag*/
  SREG = sreg;
}


BOOL compare_equal(uint16_t a, uint16_t b)
{
  BOOL ret;
  ret = ((a <= (b + OFFSET)) && (a >= (b - OFFSET)));
  return ret;
}


ISR(TIMER1_COMPA_vect)
{
  stick++;  
}
