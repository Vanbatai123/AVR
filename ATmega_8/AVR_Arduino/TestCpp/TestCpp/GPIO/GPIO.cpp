/*
 * GPIO.cpp
 *
 * Created: 7/25/2018 4:14:58 PM
 *  Author: taivb
 */ 


#include "../include.h"
#include "GPIO.h"

uint8_t pinToBit(uint8_t pin)
{
	if (pin < 8)	return pin;
	else if (pin < 14)	return pin - 8;
	else if (pin < 20)	return pin - 14;
	else	return 0;
};

volatile uint8_t *pinToDDR(uint8_t pin)
{
	if (pin < 8)	return &DDRD;
	else if (pin < 14)	return &DDRB;
	else if (pin < 20)	return &DDRC;
	return 0;
};


volatile uint8_t *pinToPort(uint8_t pin)
{
	if (pin < 8)	return &PORTD;
	else if (pin < 14)	return &PORTB;
	else if (pin < 20)	return &PORTC;
	return 0;
};

volatile uint8_t *pinToInput(uint8_t pin)
{
	if (pin < 8)	return &PIND;
	else if (pin < 14)	return &PINB;
	else if (pin < 20)	return &PINC;
	return 0;
};

void pinMode( uint8_t pin, uint8_t mode)
{
	volatile uint8_t *ddr, *port;
	uint8_t bit = pinToBit(pin);
	port = pinToPort(pin);
	ddr = pinToDDR(pin);
	
	if (mode == INPUT)	clrb(*ddr, bit);
	else if (mode == INPUT_PULLUP)
	{
		setb(*port, bit);
		clrb(*ddr, bit);
	}
	else	setb(*ddr, bit);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
	volatile uint8_t *port;
	uint8_t bit = pinToBit(pin);
	port = pinToPort(pin);
	
	if (val == 1)	setb(*port, bit);
	else	clrb(*port, bit);
}

uint8_t digitalRead(uint8_t pin)
{
	volatile uint8_t *input;
	uint8_t bit = pinToBit(pin);
	input = pinToInput(pin);
	
	if (inbit(*input, bit))	return 1;
	else	return 0;
}