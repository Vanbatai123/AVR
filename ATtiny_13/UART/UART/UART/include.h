/*
 * include.h
 *
 * Created: 4/10/2018 8:23:03 AM
 *  Author: Van_BasTai
 */ 


#ifndef INCLUDE_H_
#define INCLUDE_H_


#include "UART13/UART13.h"

#include <avr/io.h>
#define F_CPU 9600000UL
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



#endif /* INCLUDE_H_ */