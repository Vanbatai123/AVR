/*
* TEST_ATTINY13.c
*
* Created: 2/27/2017 8:41:13 PM
*  Author: Van_BasTai
*/

#include "mySTART.h"

int main(void)
{
	//DDRB = 0xFF;
	setb(DDRB, 0);
	while(1)
	{
		daobit(PORTB, 0);
		_delay_ms(500);
	}
}