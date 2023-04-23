#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define setb(regg,bit)		regg |= (1<<bit)
#define clrb(regg,bit)		regg &= (~(1<<bit))
#define inbit(regg,bit)		(regg&(1<<bit))>>bit
#define daobit(regg,bit)	regg ^= (1<<bit)

#define HEX		16
#define DEC		10
#define BIN 	2

/*
 * ADC initialization
 */
void ADC_config();

/*
 * ADC read channel function
 */
uint16_t ADC_read(uint8_t adc_channel);