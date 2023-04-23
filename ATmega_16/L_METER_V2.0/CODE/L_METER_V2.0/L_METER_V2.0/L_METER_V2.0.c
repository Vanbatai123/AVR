/*
 * L_METER_V2.0_PROJECT
 *
 * Created: 11/11/2016 11:36:00 AM
 *  Author: VAN BASTAI
 */ 

#include "E:\AVR\Header\mySTART.h"
#include "E:\AVR\Header\myUART.h"
#include "E:\AVR\Header\myTIMER_1.h"
long n;
int main(void)
{
	DDRA=0xff;	
	init_UART();
	hesochia(1);

	//NGAT NGOAI
	setb(PORTD,0);
	MCUCR=(1<<ISC01)|(1<<ISC11);// CHON SUON NGAT
	GICR=(1<<INT0)|(1<<INT1);//BAT INT0 VA INT 1
	sei();//BAT NGAT TOAN CUC
	_delay_ms(300);
	while(1)
	{
		setb(PORTA,0);_delay_ms(5);
		clrb(PORTA,0);_delay_ms(10000);
		//vietso_UART(F_CPU/n);
		//_delay_ms(500);
		//print_UART("\r");
	}
}
ISR(INT1_vect)
{
	n=TCNT1;
	TCNT1=0;
	vietso_UART(F_CPU/n);
	//_delay_ms(500);
	print_UART("\r");
}