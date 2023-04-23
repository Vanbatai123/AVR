/*
 * cube_2x2x2.c
 *
 * Created: 11/2/2015 5:43:46 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
# define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	DDRC=DDRD=0XFF;
	PORTC=PORTD=0XFF;
    while(1)
    {
		for (int g=0;g<10;g++)
		{
			PORTD=0x01;
			PORTC=~0x03;
			_delay_us(1);
			PORTC=0XFF;
			PORTD=0x02;
			PORTC=~0x04;
			_delay_us(1);
			PORTC=0XFF;
			
		}
    }
}