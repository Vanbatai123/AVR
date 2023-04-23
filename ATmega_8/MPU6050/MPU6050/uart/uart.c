/*
* UART.c
*
* Created: 10/27/2018 10:17:47 AM
*  Author: PC
*/

#include "../include.h"
#include "uart.h"

#ifdef USE_PRINTF
int usart_putchar_printf(char var, FILE *stream)
{
	if (var == '\n') UART_putChar('\r');
	UART_putChar(var);
	return 0;
}
#endif

void UART_config(uint32_t baudrate)
{
	#ifdef UBRRH	// ATmega8,16
	// Set baud rate
	UBRRH = 0;
	UBRRL = F_CPU/16/baudrate;
	//
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
	UCSRB = (1<<TXEN) | (1<<RXEN);
	#endif
	
	#ifdef UBRR0H	// ATmega328
	// Set baud rate
	UBRR0H = 0;
	UBRR0L = F_CPU/16/baudrate;
	//
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	#endif
}

void UART_putChar(char c)
{
	
	#ifdef UDR	// ATmega8,16
	// Wait for empty transmit buffer
	while (inbit(UCSRA, UDRE) == 0);
	// Start transmission
	UDR = c;
	#endif
	
	#ifdef UDR0	// ATmega328
	
	// Wait for empty transmit buffer
	while (inbit(UCSR0A, UDRE0) == 0);
	// Start transmission
	UDR0 = c;
	#endif
	
}

void UART_print(char* str)
{
	while (*str)// loop through entire string
	{
		UART_putChar(*str);
		str++;
	}
}

void UART_printNum(long num, uint8_t base)
{
	char dis[20];
	if (base == DEC)	sprintf(dis, "%li", num);
	else if (base == HEX)	sprintf(dis, "0x%lx", num);
	else if (base == BIN)	sprintf(dis, "can't print binary number");
	UART_print(dis);
}

void UART_printNumln(long num, uint8_t base)
{
	char dis[20];
	if (base == DEC)	sprintf(dis, "%li\r\n", num);
	else if (base == HEX)	sprintf(dis, "0x%lx\r\n", num);
	else if (base == BIN)	sprintf(dis, "can't print binary number\r\n");
	UART_print(dis);
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
	UART_printNum(int_part, DEC);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		UART_putChar('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		UART_printNum(toPrint, DEC);
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
	#ifdef UDR // ATmega16
	// Wait for incoming data
	while ( ( UCSRA & ( 1 << RXC ) ) == 0);
	/// Return the data
	return UDR;
	#endif
	
	#ifdef UDR0 // ATmega328
	// Wait for incoming data
	while ( ( UCSR0A & ( 1 << RXC0 ) ) == 0);
	/// Return the data
	return UDR0;
	#endif
}

// uart interrupt function
/*ISR(USART_RXC_vect)
{
data=UDR;
UART_putChar(data);
_delay_ms(50);
}*/
