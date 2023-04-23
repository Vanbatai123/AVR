/*
 * HC05.c
 *
 * Created: 1/28/2017 10:43:29 PM
 *  Author: Van_BasTai
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myUART.h"

int main(void)
{
	init_UART();
	setb(TIMSK,TOIE0);
	sei();
	vietso_dec_UART(123);
	while(1)
	{
		
	}
}
ISR(USART_RXC_vect)
{
	data=UDR;
	if(data == 50 )
	{
		print_UART("LEFT_CLICK");
		newline_UART();
	}
	else if(data == 49 )
	{
		print_UART("RIGHT_CLICK");
		newline_UART();
	}
	else if(data == 51 )	
	{
		print_UART("RELEASED");
		newline_UART();
	}
	else
	{
		dem ++;
		if (data > 127)
		data-= 256;
		if (dem % 2)
		{
			print_UART("dX = ");
			vietso_dec_UART(data);
			print_UART("\t");
		}
		else
		{
			print_UART("dY = ");
			vietso_dec_UART(data);
			newline_UART();
		}
	}
}