/*
 * DOXUNG_F_T.c
 *
 * Created: 9/23/2016 9:07:00 PM
 *  Author: Van BasTai
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#include "E:\AVR\Header\myUART.h"
//#include "E:\AVR\Header\myLCD.h"
//float tanso=0;
int tanso=0,chuki=0,L;



int main(void)
{
	clrb(DDRD, 6);  //chan ICP1(PD6) la input
	setb(PORTD, 6); //keo chan ICP1 len cao bang dien tro noi
	
	TCNT1=0;
	//TCCR1B=(1<<CS10);	   // CS12=0, CS11=0, CS10=1: chon Prescaler = 1, input capture canh xuong
	//TIMSK= (1<<TICIE1)|(1<<TOIE1);          // cho phep ngat Input Capture, ngat tran
	setb(TIMSK,TICIE1);
	//setb(TIMSK,TOIE1);
	setb(TCCR1B,CS10);
	//setb(TCCR1B,CS12);
	//setb(TCCR1B,ICES1);
	sei(); 		                              //set bit I cho phep ngat toan cuc
	_delay_ms(100);
	//UART
	init_UART();
	//print_UART("vanbatai\r");
	//vietsothapphan_UART(2.3232);
	while(1)
	{
		//chuki=chuki/8000;
		tanso=8000/chuki;
		//print_UART("T = ");
		vietso_UART(chuki);
		///print_UART(" ms");
		print_UART("      ");
		print_UART("f = ");
		vietso_UART(tanso);
		print_UART(" KHz");
		L=10000000/(39.4784*tanso*tanso);
		print_UART("      ");
		print_UART("L = ");
		vietso_UART(L);
		print_UART(" uH");
		print_UART("\r");
		_delay_ms(200);
	}
	
}
//trinh phuc vu ngat tran T/C1
/*ISR (TIMER1_OVF_vect )
{
	over_step+=0xFFFF; //tran bo dem, tang so dem them 0xFFFF
}*/
//trinh phuc vu ngat Input Capture T/C1
ISR (TIMER1_CAPT_vect)
{
	chuki=ICR1;
	TCNT1=0;
}
