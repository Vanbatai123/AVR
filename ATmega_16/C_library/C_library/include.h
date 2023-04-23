#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <util/twi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


#define setb(reg,bit)		reg |= 1<<bit
#define clrb(reg,bit)		reg &= ~(1<<bit)
#define inbit(reg,bit)		(reg & (1 << bit)) >> bit
#define daobit(reg,bit)		reg ^= 1<<bit
typedef void (*voidFuncPtr)(void);