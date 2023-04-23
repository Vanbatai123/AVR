/*
 * UART13.c
 *
 * Created: 4/10/2018 8:15:19 AM
 *  Author: Van_BasTai
 */ 

#include "../include.h"
#include "UART13.h"

void uart_tx_init ()
{
	TCCR0A = 1 << WGM01;		// compare mode
	TCCR0B = 1 << CS00;		// prescaler 1
	SOFT_TX_PORT |= SOFT_TX_PIN;
	SOFT_TX_DDR |= SOFT_TX_PIN;
	OCR0A = 75;			//115200 baudrate at prescaler 1
}

// 
void num_to_str(unsigned int value, unsigned char nDigit)
{
	switch (nDigit)
	{
		case 4:
		uart_send_byte((value / 1000) + '0');
		case 3:
		uart_send_byte(((value / 100) % 10) + '0');
		case 2:
		uart_send_byte(((value / 10) % 10) + '0');
		case 1:
		uart_send_byte((value % 10) + '0');
	}
}

void uart_print(char *str)
{
	unsigned char i = 0;
	while (str[i]) 
{
		uart_send_byte(str[i++]);
	}
}

//bitbanged UART transmit byte
void uart_send_byte (unsigned char data)
{
	unsigned char i;
	TCCR0B = 0;
	TCNT0 = 0;
	TIFR0 |= 1 << OCF0A;
	TCCR0B |= (1 << CS00);
	TIFR0 |= 1 << OCF0A;
	SOFT_TX_PORT &= ~SOFT_TX_PIN;
	while (!(TIFR0 &(1 << OCF0A)));
	TIFR0 |= 1 << OCF0A;
	for (i = 0; i < 8; i++)
	{
		if (data &1)
		SOFT_TX_PORT |= SOFT_TX_PIN;
		else
		SOFT_TX_PORT &= ~SOFT_TX_PIN;
		data >>= 1;
		while (!(TIFR0 &(1 << OCF0A)));
		TIFR0 |= 1 << OCF0A;
	}
	SOFT_TX_PORT |= SOFT_TX_PIN;
	while (!(TIFR0 &(1 << OCF0A)));
	TIFR0 |= 1 << OCF0A;
}


//bitbanged UART transmit byte
void uart_send_byte (unsigned char data)
{
	unsigned char i;
	TCNT0 = 0;
	TIFR0 |= 1 << OCF0A;
	SOFT_TX_PORT &= ~SOFT_TX_PIN;
	while (!(TIFR0 &(1 << OCF0A)));
	TIFR0 |= 1 << OCF0A;
	for (i = 0; i < 8; i++)
	{
		if (data &1)
		SOFT_TX_PORT |= SOFT_TX_PIN;
		else
		SOFT_TX_PORT &= ~SOFT_TX_PIN;
		data >>= 1;
		while (!(TIFR0 &(1 << OCF0A)));
		TIFR0 |= 1 << OCF0A;
	}
	SOFT_TX_PORT |= SOFT_TX_PIN;
	while (!(TIFR0 &(1 << OCF0A)));
	TIFR0 |= 1 << OCF0A;//CLEAR FLAG
}
