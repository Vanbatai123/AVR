/*
 * UART.c
 *
 * Created: 7/21/2017 8:07:36 AM
 *  Author: Van_BasTai
 */ 


#include "include.h"


int helloHabr = 0;

int main (void) 
{
	uart_tx_init ();

	while (1) 
	{
		helloHabr++;
		uart_print("Hello Habr, I'm ATtiny13 ");
		num_to_str(helloHabr, 4);
		uart_print(" counds");
		uart_print("\r\n");
		_delay_ms(1000);
	}
	return 0;
}
