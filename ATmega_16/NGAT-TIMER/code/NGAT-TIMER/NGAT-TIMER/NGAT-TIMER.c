/*
 * NGAT_TIMER.c
 *
 * Created: 11/25/2015 3:23:49 PM
 *  Author: Administrator
 */ 

#include "mySTART.h"
#include "myLCD.h"
#include "myTimer_0.h"
int ngat=0;
int demm=0;
int main(void)
{
	PORTB|=(1<<0)|(1<<1);
	DDRB|=(0<<0)|(0<<1);
	setb(DDRD, 4);
	Init_Timer_0(1024);
	init_LCD();
	TCNT0=100;
	setb(TIMSK,TOIE0);
	sei();
	while(1)
	{
		if (TCNT0 == demm)
			setb(PORTD, 4);
	}
}
ISR(TIMER0_OVF_vect)
{
	clrb(PORTD, 4);
	if (inbit(PINB, 0) == 0)
		demm++;
	if (inbit(PINB, 1) == 0)
		demm--;
	move_LCD(1,1);
	vietso_LCD(demm);
	print_LCD("   ");
	TCNT0 = 0;
}