/*
 * TEST_ATMEGA8.c
 *
 * Created: 2/9/2017 8:14:06 PM
 *  Author: Administrator
 */ 

#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = DDRB = DDRC = 0xFF;
    while(1)
    {
        PORTB = PORTC = PORTD = 0xAA;
		_delay_ms(1000);
		PORTB = PORTC = PORTD = 0x55;
		_delay_ms(1000);
    }
}