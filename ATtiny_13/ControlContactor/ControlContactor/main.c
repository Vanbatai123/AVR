/*
* ControlContactor.c
*
* Created: 11/22/2018 9:05:12 PM
* Author : PC
*/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define setb(reg,bit)		reg |= 1<<bit
#define clrb(reg,bit)		reg &= ~(1<<bit)
#define inbit(reg,bit)		(reg & (1 << bit)) >> bit

#define PIN_RF_RED			0
#define PIN_RF_WHITE		1

#define PIN_RELAY_WHITE		3
#define PIN_RELAY_RED		4

int main(void)
{
	setb(PORTB, PIN_RF_WHITE);
	setb(PORTB, PIN_RF_RED);
	
	clrb(DDRB, PIN_RF_WHITE); 
	clrb(DDRB, PIN_RF_RED);
	setb(DDRB, PIN_RELAY_WHITE);
	setb(DDRB, PIN_RELAY_RED);
	
	/* Replace with your application code */
	while (1)
	{
		if (inbit(PINB, PIN_RF_WHITE))
		{
			setb(PORTB, PIN_RELAY_WHITE);
			_delay_ms(1000);
			clrb(PORTB, PIN_RELAY_WHITE);
			_delay_ms(1000);
		}
		if (inbit(PINB, PIN_RF_RED))
		{
			setb(PORTB, PIN_RELAY_RED);
			_delay_ms(1000);
			clrb(PORTB, PIN_RELAY_RED);
			_delay_ms(1000);
		}
	}
}

