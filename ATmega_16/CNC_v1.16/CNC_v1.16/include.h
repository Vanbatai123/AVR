#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define setb(reg,bit)		reg|=(1<<bit)
#define clrb(reg,bit)		reg&=(~(1<<bit))
#define inbit(reg,bit)		(reg&(1<<bit))>>bit
#define daobit(reg,bit)		reg^=(1<<bit)

struct gValue
{
	uint8_t G;
	float R;
	int32_t X, Y, Z, I, J, F, S;
};
//9068 bytes
#define truee	1
#define falsee	0