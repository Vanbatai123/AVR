/*
 * LEDQUAY.c
 *
 * Created: 8/16/2016 8:54:00 AM
 *  Author: Administrator
 */ 


 /*
 * Created: 8/24/2015 9:48:39 AM
 *  Author: Administrator
 */
  # define F_CPU 8000000UL
  #include <avr/io.h>
  #include <util/delay.h>
  int code1[]={0xE1,0xC0,0x80,0x01,0x80,0xC0,0xE1,0xFF,0XFF,0XFF,0XFF};
  int code[]={0xFE,0xFE,0x80,0xFE,0xFE,0xFF,0x83,0xF5,0xF6,0xF5,0x83,0xFF,0xFF,0xBE,0x80,0xBE,0xFF,0xFF};
  int main(void)
  {
	  DDRD=0XFF;
	  PORTD=0xFF;	
	  while(1)
	  {
		  for (int x=0;x<18;x++)
		  {
			  PORTD=code[x];_delay_ms(2);
		  }
		  _delay_ms(36);
	 }
  }