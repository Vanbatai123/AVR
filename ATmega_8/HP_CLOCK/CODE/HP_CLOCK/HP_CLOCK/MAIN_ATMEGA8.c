/*
 * HP_CLOCK.c
 *
 * Created: 8/13/2017 11:57:57 PM
 *  Author: Van_BasTai
 */ 


#include "mySTART.h"
#include "myLCD_HP.h"
#include "Solar2Lunar.h"
#include "myDS1307.h"

int main(void)
{
	init_LCD();
	init_DS1307();
	//DS1307_setup(2,14,8,17,20,0,0);
	//DS1307_get();
	//DS1307_display();
	while(1)
	{
		ghi(0x00);
		doc();
		Display();	
		/*convertSolar2Lunar(14, 8, 2000+17);
		move_LCD(1, 1);
		vietso_dec_LCD(lunar_date);
		putchar_LCD('-');
		vietso_dec_LCD(lunar_month);
		putchar_LCD('-');
		vietso_dec_LCD(lunar_year);*/
	}
}
