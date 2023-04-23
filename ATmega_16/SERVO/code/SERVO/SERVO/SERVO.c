/*
 * SERVO.c
 *
 * Created: 11/27/2015 9:00:53 AM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#include <avr/interrupt.h>
int ngat=0;
int demm=0;
int xxx=6;
int yyy=24;
int main(void)
{
	DDRC=0XFF;
	DDRB=0X00;
	PORTB=0xff;
	setb(TCCR0,CS01);
	//setb(TCCR0,CS01);
	TCNT0=156;
	setb(TIMSK,TOIE0);//CHO PHEP NGAT
	sei();//NGAT TOAN CUC
	while(1)
	{
		
		
		if (xxx==23)
		{
			for(int i=23;i>6;i--)
			{
				xxx--;
				_delay_ms(100);
			}
		}
		if (xxx==6)
		{
			for(int i=6;i<23;i++)
			{
				xxx++;
				_delay_ms(100);
			}
		}
	}
}
ISR(TIMER0_OVF_vect)//VECTO NGAT
{
	TCNT0=156;
	
	ngat++;
	if (ngat>=xxx)
		{
			clrb(PORTC,7);
			
		}
	if(ngat>=(30-xxx))
		{
			clrb(PORTC,6);
			
		}
	if(ngat==200)
		{
			ngat=0;
			setb(PORTC,6);
			setb(PORTC,7);
			
		}	
	
}
