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

#include "LCD_HP/LCD_HP.h"
