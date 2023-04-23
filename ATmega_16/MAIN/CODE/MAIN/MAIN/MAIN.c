/*
 * MAIN.c
 *
 * Created: 1/28/2017 10:43:29 PM
 *  Author: Van_BasTai
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myTIMER_0.h"
#include "F:\AVR\Header\myADC.h"



int main(void)
{
	_delay_ms(500);
	int div=1024;
	Init_Timer_0(div);
	init_ADC();
	init_LCD();
	setb(TIMSK,TOIE0);
	sei();
	
	while(1)
	{
		
	}
}
ISR(TIMER0_OVF_vect)
{
	
}