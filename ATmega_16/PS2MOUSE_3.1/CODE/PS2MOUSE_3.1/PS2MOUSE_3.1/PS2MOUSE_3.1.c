/*
 * PS2MOUSE.c
 * NON-INTERRUP
 * Created: 12/30/2016 5:16:15 PM
 *  Author: Van_BasTai
 */ 

#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myPS2MOUSE.h"


int main(void)
{
	_delay_ms(500);
	init_LCD();
	mouse_intel();
	while(1)
	{
		mouse_all();
		move_LCD(1,1);
		vietso_bin_LCD(mstat);
		move_LCD(2,1);
		print_LCD(" x reg: ");
		vietso_dec_LCD(change_x);
		space_LCD(3);
		
		move_LCD(3,1);
		print_LCD(" x position: ");
		vietso_dec_LCD(position_x);
		space_LCD(3);
		
		move_LCD(4,1);
		print_LCD(" y reg: ");
		vietso_dec_LCD(change_y);
		space_LCD(3);
		
		move_LCD(5,1);
		print_LCD(" y position: ");
		vietso_dec_LCD(position_y);
		space_LCD(3);
		
		move_LCD(6,1);
		print_LCD(" z reg: ");
		vietso_dec_LCD(change_z);
		space_LCD(3);
		
		move_LCD(7,1);
		print_LCD(" z position: ");
		vietso_dec_LCD(position_z);
		space_LCD(3);
		
		_delay_ms(200);
	}//end while(1)
}//end main
