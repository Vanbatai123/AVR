/*
 * IR_DECODE.c
 *
 * Created: 1/28/2017 10:43:29 PM
 *  Author: Van_BasTai
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myUART.h"
#include "F:\AVR\Header\myTIMER_1.h"

long int IR_ma,time;
char count;

int main(void)
{
	
	Init_Timer_1(64);
	init_UART();
	vietso_dec_UART(123);
	MCUCR=(1<<ISC01)|(1<<ISC11);
	GICR=(1<<INT0)|(1<<INT1);
	sei();
	while(1)
	{
		
	}
}
ISR(INT1_vect)
{
	count++;
	time=TCNT1;
	TCNT1=0;
	if(count>18)
	{
		if ((time > 300)&&(time < 1300))
			IR_ma = (IR_ma<<1) + 1;
		else if(time < 300)
			IR_ma = IR_ma<<1;
		else if((time > 2000)&&(time < 6000))
		{
			vietso_hex_UART(IR_ma);
			IR_ma = 0;
			count = 0;
			newline_UART();
		}
	}
}