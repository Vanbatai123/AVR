//#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define UART_BAUD_CALC(UART_BAUD_RATE,MPUCLK) ((MPUCLK)/((UART_BAUD_RATE)*16L)-1)

void (*rxfunc) (const char c) = NULL;

ISR(USART_RXC_vect)
{
    if (NULL != rxfunc) {
	rxfunc(UDR);
    }
}

void usart_init(void (*p) (const char c))
{
    UCSRB = 0x00;
    UCSRA = 0x00;
    UCSRC = (1 << URSEL) | 0x06;
    uint16_t bcnt = UART_BAUD_CALC(19200, F_CPU);
    UBRRH = (uint8_t) ((bcnt >> 8) & 0xff);
    UBRRL = (uint8_t) ((bcnt >> 0) & 0xff);
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
    rxfunc = p;
}

void usart_tx(char data)
{
    while (0 == (UCSRA & (1 << UDRE))) {
    }
    UDR = data;
}
void xprintf(char* str)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; str[i]!=0; i++) 
	usart_tx(str[i]);
}
