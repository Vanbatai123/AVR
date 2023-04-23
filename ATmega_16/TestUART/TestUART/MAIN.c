/*
* TestUART.c
*
* Created: 1/28/2017 10:43:29 PM
*  Author: Van_BasTai
*/


#include "mySTART.h"
#include "myUART.h"
#include "myLCD_HP.h"

#define ERROR 0x16
int x = 0;

int main(void)
{
	init_LCD();
	init_UART();
	move_LCD(1,1);
	vietso_dec_LCD(1);
	sei();
	while(1)
	{
		//clr_LCD();
		putchar_UART(123);
		_delay_ms(300);
		putchar_UART(0x73);
		_delay_ms(300);
	}
}
/************************************************************************/
/* UART Interrupt - Receive data                                        */
/************************************************************************/
ISR(USART_RXC_vect)
{
	x = UDR;
	
}