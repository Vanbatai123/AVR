/*
 * DS1307_BY_ME.c
 *
 * Created: 4/20/2017 08:51:57 PM
 *  Author: Van_BasTai
 */ 

#include "mySTART.h"
#include "myLCD.h"
#include "Solar2Lunar.h"
#include "myDS1307.h"

int main(void)
{
	init_LCD();
	DS1307_init();
	while(1)
	{
		DS1307_get();
		DS1307_display();
		convertSolar2Lunar(date, month, 2000+year);
		move_LCD(1, 10);	
		vietso_LCD(lunar_date);
		putChar_LCD('-');
		move_LCD(1, 13);
		vietso_LCD(lunar_month);
		putChar_LCD('-');
		move_LCD(2, 13);
		vietso_LCD(lunar_year);
	}
}
