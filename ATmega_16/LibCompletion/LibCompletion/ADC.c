/*
 * include.c
 *
 * Created: 8/31/2018 1:39:44 PM
 *  Author: ASUS PC
 */ 

#include "include.h"

uint16_t ADC_read(uint8_t adc_channel)
{
	ADMUX = adc_channel | 0x40;
	ADCSRA |= (1<<ADSC); 				//start conversion
	while(inbit(ADCSRA,ADIF) == 0);		//wait until ADIF==1
	return ADCW;
}
void ADC_config()
{
	setb(ADMUX, REFS0);
	ADCSRA = (1<<ADEN) | (1<<ADPS2);
}