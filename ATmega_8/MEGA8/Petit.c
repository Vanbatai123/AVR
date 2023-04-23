#include <mega8.h>
#include "diskio.h"
#include "pff.h"

unsigned char buffer[512];   /* буфер дл€ копировани€ данных файла */
volatile unsigned int count;

interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
    OCR1A = buffer[count];
	if (++count >= 512)
		count = 0;
}

void main(void)
{    
    unsigned int br;         /* счетчик чтени€/записи файла */      
    unsigned char buf = 0;    
    FATFS fs;         /* –абоча€ область (file system object) дл€ логических дисков */
                  
PORTB=0x00;
DDRB=0x02;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 8000,000 kHz
// Mode: Fast PWM top=0x00FF
// OC1A output: Non-Inv.
TCCR1A=0x81;
TCCR1B=0x09;
TCNT1H=0x00;
TCNT1L=0x00;
OCR1A=0x00;


// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 250,000 kHz
// Mode: CTC top=OCR2
// OC2 output: Disconnected
TCCR2=0x0B;
TCNT2=0x00;
//OCR2=0x1F; //дл€ 8к√ц
OCR2=0xB;    //дл€ 22к√ц

#asm("sei")
// Timer(s)/Counter(s) Interrupt(s) initialization

 
if(disk_initialize()==0) //инициализируем флешку
{
  pf_mount(&fs);  //монтируем файловую систему      
  pf_open("1.wav");  //открываем вавку 
  pf_lseek(44);     //перемещаем указатель на 44  
  pf_read(buffer, 512,&br);
  TIMSK=0x80;
  while(1)
  {
     if(!buf && count>255)
     {
        pf_read(&buffer[0], 256,&br);
        buf=1; 
        if (br < 256)
	break;
     }  
     if(buf && count<256)
     {
        pf_read(&buffer[256], 256,&br);
        buf = 0; 
        if (br < 256)
	break;
     }
     
  }
  TIMSK = 0x00;
  pf_mount(0x00);
}

while (1)
      {
      

      }
}
