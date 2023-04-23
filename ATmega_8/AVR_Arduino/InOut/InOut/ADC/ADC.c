/*
 * ADC.c
 *
 * Created: 7/25/2018 3:59:12 PM
 *  Author: taivb
 */ 

#include "../include.h"
#include "ADC.h"


uint16_t analogRead(uint8_t pin)
{
	if (pin >= 14) pin -= 14; // convert pin to chanel
	
	//config ADC
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2);
	
	ADMUX = pin | 0x40;	//select chanel
	ADCSRA |= (1 << ADSC); // start conversion
	
	while(inbit(ADCSRA, ADIF) == 0); //wait until conversion completed
	
	return ADCW;
}
