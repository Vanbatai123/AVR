/*
* UART.c
*
* Created: 10/27/2018 10:17:47 AM
*  Author: PC
*/

#include "../include.h"
#include "uart.h"

int usart_putchar_printf(char var, FILE *stream)
{
	if (var == '\n') UART_putChar('\r');
	UART_putChar(var);
	return 0;
}

void UART_config(uint32_t baudrate)
{
	// Set baud rate
	UBRRH = 0;
	UBRRL = F_CPU/16/baudrate;
	//
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
}

void UART_putChar(char c)
{
	// Wait for empty transmit buffer
	while (inbit(UCSRA,UDRE) == 0);
	// Start transmission
	UDR = c;
}

void UART_print(char* str)
{
	// loop through entire string
	while (*str)
	{
		UART_putChar(*str);
		str++;
	}
}

void UART_printNum(long num)
{
	char dis[20];
	sprintf(dis, "%li", num);
	UART_print(dis);
}

void UART_printNumln(long num)
{
	UART_printNum(num);
	UART_print("\r\n");
}

void UART_printFloat(double num, uint8_t digits)
{
	if (isnan(num)) return UART_print("nan");
	if (isinf(num)) return UART_print("inf");
	if (num > 4294967040.0) return UART_print ("ovf");  // constant determined empirically
	if (num <-4294967040.0) return UART_print ("ovf");  // constant determined empirically
	
	// Handle negative numbers
	if (num < 0.0)
	{
		UART_putChar('-');
		num = -num;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;
	
	num += rounding;

	// Extract the integer part of the num and print it
	unsigned long int_part = (unsigned long)num;
	double remainder = num - (double)int_part;
	UART_printNum(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		UART_putChar('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		UART_printNum(toPrint);
		remainder -= toPrint;
	}
}

void UART_printFloatln(double num, uint8_t digits)
{
	UART_printFloat(num, digits);
	UART_print("\r\n");
}

void UART_println(char* str)
{
	UART_print(str);
	UART_print("\r\n");
}

uint8_t UART_getChar()
{
	// Wait for incoming data
	while ( ( UCSRA & ( 1 << RXC ) ) == 0);
	/// Return the data
	return UDR;
}

void UART_printXY(int32_t x, int32_t y)
{
	UART_print("X = ");
	UART_printNum(x);
	UART_print("\tY = ");
	UART_printNumln(y);
}

// uart interrupt function
/*ISR(USART_RXC_vect)
{
data=UDR;
UART_putChar(data);
_delay_ms(50);
}*/
