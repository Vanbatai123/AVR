/*
 * IR_REMOTE.c
 *
 * Created: 5/25/2017 11:14:53 AM
 *  Author: Van_BasTai
 */ 


#include "mySTART.h"
#include "myLCD_HP.h"
#include "mytimer_1.h"

unsigned int IR_ma,timer, IR_all;
char count;

int main(void)
{
	Init_Timer_1(8);
	init_LCD();
	MCUCR|=(0<<ISC01)|(1<<ISC11);
	GICR|=(1<<INT1);
	sei();
	while(1)
	{
		move_LCD(1,1);
		vietso_hex_LCD(IR_all);
	}
}
ISR(INT1_vect)
{
	count++;
	timer = TCNT1/200;
	TCNT1 = 0;
	if ((timer > 110)&&(timer < 120))
	{
		count = 0;
		IR_all = IR_ma;
	}
	if(count > 18)
	{
		if ((timer > 20)&&(timer < 30))
			IR_ma = (IR_ma<<1) + 1;
		else if(timer < 15)
			IR_ma = IR_ma<<1;
	}
}