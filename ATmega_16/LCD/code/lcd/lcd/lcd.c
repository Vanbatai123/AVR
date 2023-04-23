/*
 * lcd.c
 *
 * Created: 10/27/2015 8:11:05 AM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "F:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit

int main(void)
{
	
	init_LCD();
	clr_LCD();
    while(1)
    {
		move_LCD(1,1);
		print_LCD("van ba tai");
    }
}