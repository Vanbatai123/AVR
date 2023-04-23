/*
 * Test2.c
 *
 * Created: 2/10/2017 7:14:53 PM
 *  Author: Van_BasTai
 */ 


#include "E:\AVR\Header\mySTART.h"



int main(void)
{
	DDRD = 0xFF;
	setb(DDRC, 6);
	while(1)
	{
		setb(PORTC, 6);
		PORTD = 0xFF;
		_delay_ms(300);
		PORTD = 0x00;
		_delay_ms(300);
	}
}
ISR(TIMER0_OVF_vect)
{
	
}