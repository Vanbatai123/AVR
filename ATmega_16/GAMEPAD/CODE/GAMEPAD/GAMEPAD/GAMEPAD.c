/*
 * PS2GAMEPAD.c
 *
 * Created: 12/30/2016 7:43:27 PM
 *  Author: Van_BasTai
 */ 

#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myPS2GAMEPAD.h"

int main(void)
{
	_delay_ms(500);
	init_LCD();
	vietso_dec_LCD(55);
	init_GAMEPAD();
	while (1)
	{
		read_GAMEPAD();
	} // while(1)
} //main
