/*
 * Ngat_Ngoai.c
 *
 * Created: 1/16/2016 9:36:16 AM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
int x,y;
int main(void)
{
	
	
	DDRC=0xff;
	DDRB=0xff;
	DDRD=0x00;
	setb(PORTD,0);
	MCUCR=(1<<ISC01)|(1<<ISC11);
	GICR=(1<<INT0)|(1<<INT1);
	sei();
	init_LCD();
	print_LCD("XUNG = ",7);
	x=0;
    while(1)
    {
		move_LCD(1,8);
		vietso(x);
		move_LCD(2,1);
		if (y==0)
		{
			
			print_LCD("T",1);
		}
		else
			print_LCD("N",1);
		
    }
}
ISR(INT0_vect)
{
	x++;	         //n?u có ng?t INT0 x?y ra, t?ng val thêm 1
	if (inbit(PIND,0)==0)
	{
		y=0;
	}
	else
		y=1;                      
}
