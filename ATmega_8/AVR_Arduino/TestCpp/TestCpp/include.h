/*
 * include.h
 *
 * Created: 7/25/2018 10:27:08 AM
 *  Author: taivb
 */ 


#ifndef INCLUDE_H_
#define INCLUDE_H_

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


#define delay(ms)				_delay_ms(ms)
#define delayMicroseconds(us)	_delay_us(us)
typedef unsigned char byte;

#include "GPIO/GPIO.h"
#include "Wire/Wire.h"
#include "Serial/Serial.h"
#include "Liquid/LiquidCrystalLCD.h"


#endif /* INCLUDE_H_ */