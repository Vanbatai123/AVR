/*
* UART_v2.0.c
*
* Created: 04/10/2018 8:32:36 AM
*  Author: Van_BasTai
*/

#include "include.h"
#define LEDPIN	1
/*
		PB0----5-----
		PB1----6-----
		PB2----7-----
		PB3----2-----TX
		PB4----3-----RX
*/

int main(void)
{
	setb(DDRB, LEDPIN);
	setb(PORTB, LEDPIN);
	char c;
	while (1)
	{
		c = uart_getc();
		if(c == 125)
		{
			clrb(PORTB, LEDPIN);
			c = 124;
		}
		else if(c == 124)
		{
			setb(PORTB, LEDPIN);
		}
	}
}

