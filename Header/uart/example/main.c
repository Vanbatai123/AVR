/*
* CNC_v1.0.c
*
* Created: 10/25/2018 9:14:53 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "stepper/stepper.h"
#include "UART/UART.h"

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	UART_config(115200);
	stdout = &mystdout;
	UART_print("das");
	UART_printNum(65534, HEX);
	UART_println("");
	UART_printNum(5555, DEC);
	UART_println("");
	UART_printNum(-5555, DEC);
	UART_println("");
	UART_printFloat(14.15, 2);
	UART_println("");
	UART_printFloat(-14.15, 2);
	UART_println("");
	printf("dasd %d",123);
	UART_println("");
	stepper_config();
	int Index;
	while(1)
	{
		
	}
	while(0)
	{
		for(Index = 0; Index < 3200; Index++)
		{
			stepper_ctrl(1, 0);
		}
		_delay_ms(1000);
		for(Index = 0; Index < 3200; Index++)
		{
			stepper_ctrl(0, 1);
		}
		_delay_ms(1000);
		for(Index = 0; Index < 3200; Index++)
		{			
			stepper_ctrl(-1, -1);
		}
		_delay_ms(1000);
	}
}