/*
 * PS2KEYBOARD.c
 *
 * Created: 1/8/2017 7:54:37 PM
 *  Author: Administrator
 */ 

#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myUART.h"

#define DATA			3
#define CLK				2
#define PORT_PS2		PORTD
#define PIN_PS2			PIND
#define DDR_PS2			DDRD
unsigned char data,bitcount=11;   // holds the received scan code
// keyboard scan codes (without & with shift key pressed)
unsigned char unshifted[67][2] = 
{
	//0x0d,9,
	{0x0e,'`'},{0x15,'q'},{0x16,'1'},{0x1a,'z'},{0x1b,'s'},{0x1c,'a'},{0x1d,'w'},{0x1e,'2'},{0x21,'c'},{0x22,'x'},{0x23,'d'},{0x24,'e'},
	{0x25,'4'},{0x26,'3'},{0x29,' '},{0x2a,'v'},{0x2b,'f'},{0x2c,'t'},{0x2d,'r'},{0x2e,'5'},{0x31,'n'},{0x32,'b'},{0x33,'h'},{0x34,'g'},
	{0x35,'y'},{0x36,'6'},{0x39,','},{0x3a,'m'},{0x3b,'j'},{0x3c,'u'},{0x3d,'7'},{0x3e,'8'},{0x41,','},{0x42,'k'},{0x43,'i'},{0x44,'o'},
	{0x45,'0'},{0x46,'9'},{0x49,'.'},{0x4a,'/'},{0x4b,'l'},{0x4c,';'},{0x4d,'p'},{0x4e,'-'},{0x52,'`'},{0x54,'['},{0x55,'='},{0x5a, 13},
	{0x5b,']'},{0x5d,'/'},{0x61,'<'},{0x66,  8},{0x69,'1'},{0x6b,'4'},{0x6c,'7'},{0x70,'0'},{0x71,','},{0x72,'2'},{0x73,'5'},{0x74,'6'},
	{0x75,'8'},{0x79,'+'},{0x7a,'3'},{0x7b,'-'},{0x7c,'*'},{0x7d,'9'},{0   ,  0}
};
unsigned char shifted[67][2] = 
{
	//0x0d,9,
	{0x0e,'`'},{0x15,'Q'},{0x16,'!'},{0x1a,'Z'},{0x1b,'S'},{0x1c,'A'},{0x1d,'W'},{0x1e,'@'},{0x21,'C'},{0x22,'X'},{0x23,'D'},{0x24,'E'},
	{0x25,'$'},{0x26,'#'},{0x29,' '},{0x2a,'V'},{0x2b,'F'},{0x2c,'T'},{0x2d,'R'},{0x2e,'%'},{0x31,'N'},{0x32,'B'},{0x33,'H'},{0x34,'G'},
	{0x35,'Y'},{0x36,'^'},{0x39,'L'},{0x3a,'M'},{0x3b,'J'},{0x3c,'U'},{0x3d,'&'},{0x3e,'*'},{0x41,'<'},{0x42,'K'},{0x43,'I'},{0x44,'O'},
	{0x45,')'},{0x46,'('},{0x49,'>'},{0x4a,'?'},{0x4b,'L'},{0x4c,':'},{0x4d,'P'},{0x4e,'_'},{0x52,'"'},{0x54,'{'},{0x55,'+'},{0x5a,13},
	{0x5b,'}'},{0x5d,'|'},{0x61,'>'},{0x66,8}  ,{0x69,'1'},{0x6b,'4'},{0x6c,'7'},{0x70,'0'},{0x71,','},{0x72,'2'},{0x73,'5'},{0x74,'6'},
	{0x75,'8'},{0x79,'+'},{0x7a,'3'},{0x7b,'-'},{0x7c,'*'},{0x7d,'9'},{0   ,0  }
};
// decode scan code
void decode(unsigned char sc) 
{
    static unsigned char is_up=0, shift = 0, mode = 0;
	unsigned char i;
	if (!is_up) 
	{
		switch (sc) 
		{
			case 0xF0 :// The up-key identifier
				is_up = 1;
				break;
			case 0x12 :// Left SHIFT
				shift = 1;
				break;
			case 0x59 :// Right SHIFT
				shift = 1;
				break;
			case 0x05 :// F1
				if(mode == 0)
				   mode = 1;// Enter scan code mode
				if(mode == 2)
				   mode = 3;// Leave scan code mode
				break;
			default:
            if(mode == 0 || mode == 3) // If ASCII mode         
			{   
				if(!shift) // If shift not pressed, do a table look-up
				{
					for(i = 0; unshifted[i][0]!=sc && unshifted[i][0]; i++);
						if (unshifted[i][0] == sc) 
					{
                        putchar_UART(unshifted[i][1]);                     
                    }                                       
				}
				else // If shift pressed
				{
					for(i = 0; shifted[i][0]!=sc && shifted[i][0]; i++);
						if (shifted[i][0] == sc) 
						{
							putchar_UART(shifted[i][1]);
						}
				}
            }
            else putchar_UART(sc);   // scan code mode (debugging mode)                        
            break;
      }//end switch
   }
   else 
   {
      is_up = 0;// Two 0xF0 in a row not allowed
      switch (sc) 
	  {
         case 0x12 :// Left SHIFT
            shift = 0;
            break;
         case 0x59 :// Right SHIFT
            shift = 0;
            break;
         case 0x05 :// F1 --  F1 puts you in debugging mode
               // pressing F1 again gets you out of debugging mode
               // in debugging mode hex code of the scan codes
               // are stored in the buffer instead of their ascii codes      
            if(mode == 1)
            mode = 2;
            if(mode == 3)
               mode = 0;
            break;      
      }
   }//end else
}                                               
//***********************************************
int main(void)
{
	MCUCR=1<<ISC01;
	GICR=1<<INT0;
	init_UART();
	sei();
while (1)
      {

      }
}
ISR(INT0_vect)
{
	// function entered at falling edge of the kbd clock signal
	// if data bit is the next bit to be read
	// (bit 3 to 10 is data, start, stop & parity bis are ignored
	if((bitcount < 11) && (bitcount > 2))
	{
		data = (data >> 1);
		if (inbit(PIN_PS2,DATA) == 1)         // if next bit is 1
			data = data | 0x80;   // store a '1'
		else
			data = data & 0x7f; // else store a '0'
	}
	if(--bitcount == 0)// all bits received ?
	{      
		decode(data);       // decode received byte
		bitcount = 11;      // reset bit counter

	}

	
}