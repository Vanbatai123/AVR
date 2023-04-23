/*
 * lcd.c
 *
 * Created: 10/27/2015 8:11:05 AM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
unsigned char chuoi[]={' ',' ','D','i','e','n',' ','T','u',' ','D','B','O','T',' ',' ',' ',' '};
int main(void)
{
	DDRC=0xff;
	DDRD=0xff;
	init_LCD();
	
    while(1)
    {
		move_LCD(1,17);
		
		for (int i=0;;i++)
		{
			//clr_LCD();
			//move_LCD(1,i);
			putChar_LCD(chuoi[i]);//sua thu vvien LCD
			_delay_ms(90);
			if (i==15)
			i=-1;
		}
		
	}
}