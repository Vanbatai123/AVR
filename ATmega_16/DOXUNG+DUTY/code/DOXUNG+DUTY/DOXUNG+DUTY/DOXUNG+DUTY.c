/*
 * DOXUNG_DUTY.c
 *
 * Created: 9/23/2016 10:47:56 PM
 *  Author: AdministraT0r
 */ 

#include "E:\AVR\Header\mySTART.h"
#include "E:\AVR\Header\myUART.h"
float tanso=0;
float chuki=0,T0=0,T1=0;
long duty=0,high=0,low=0;

int main(void)
{
	clrb(DDRD, 6);  //chan ICP1(PD6) la input
	setb(PORTD, 6); //keo chan ICP1 len cao bang dien tro noi
	
	TCNT1=0;
	setb(TIMSK,TICIE1);			//NGAT INPUT CAPTURE
	//setb(TIMSK,T0IE1);		//NGAT TRAN
	setb(TCCR1B,CS10);			//
	//setb(TCCR1B,CS12);			//PRESCALE 1024
	//setb(TCCR1B,ICES1);		//SUON KICH
	sei(); 		                              //set bit I cho phep ngat T0an cuc
	_delay_ms(100);
	//UART
	init_UART();
	while(1)
	{
		// TINH CHU KI VA TAN SO
		chuki=(high+low)/12;
		tanso=1000/chuki;
		T0=(high)/12;
		T1=(low)/12;
		duty=100*high/(high+low);
		print_UART("T = ");
		vietso_UART(chuki);
		print_UART("us   ");
		
		print_UART("To = ");
		vietso_UART(T0);
		print_UART("us   ");
		
		print_UART("T1 = ");
		vietso_UART(T1);
		print_UART("us   ");
		
		print_UART("f = ");
		vietso_UART(tanso);
		print_UART("Hz   ");
		// TINH DUTY CYCLE
		print_UART("Duty cycle = ");
		vietso_UART(duty);
		print_UART("%");
		
		print_UART("    v = ");
		vietso_UART(60000000/((high+low)*120));
		print_UART("v/p   ");
		
		print_UART("\r");
		_delay_ms(150);	
	}
	
}
ISR (TIMER1_CAPT_vect)
{
	if (inbit(TCCR1B,ICES1)==0)
	{
		high=ICR1;
	}
	else
	{
		low=ICR1;
	}
	daobit(TCCR1B,ICES1);
	TCNT1=0;
}
