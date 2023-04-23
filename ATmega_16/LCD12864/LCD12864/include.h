/*
 * include.h
 *
 * Created: 7/25/2018 10:27:08 AM
 *  Author: taivb
 */ 


#ifndef INCLUDE_H_
#define INCLUDE_H_


///////////////////
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))


#define CHANGE	1
#define FALLING 2
#define RISING	3

#define delay(ms)				_delay_ms(ms)
#define delayMicroseconds(us)	_delay_us(us)
typedef unsigned char byte;

#define _BV(bit) (1 << (bit))

typedef void (*voidFuncPtr)(void);

void init();
void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);



#define delay(ms)				_delay_ms(ms)
#define delayMicroseconds(us)	_delay_us(us)
typedef unsigned char byte;


#define interrupts() sei()
#define noInterrupts() cli()


#endif /* INCLUDE_H_ */