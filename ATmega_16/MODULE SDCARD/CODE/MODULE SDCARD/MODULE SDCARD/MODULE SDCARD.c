#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
int n=0;

int main(void)
{
	DDRB=0x00;
	PORTB=0xff;
	DDRD=0xff;
	PORTD=0x00;
	//TCCR1A=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	//TCCR1B=(1<<WGM12)|(1<<WGM13)|(1<<CS10);
	//ICR1=200;
	TIMSK=(1<<TOIE1);
	sei();
	while(1)
	{

	}
}
ISR(TIMER0_OVF_vect)
{
	
}	