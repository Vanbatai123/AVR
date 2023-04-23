/*
 * DS1307.c
 *
 * Created: 2/10/2017 7:14:53 PM
 *  Author: Van_BasTai
 */ 


#include "mySTART.h"
#include "myLCD_HP.h"
#include "myDS1307.h"


int main(void)
{
	init_LCD();
	init_DS1307();
	ghi(reg);
	while(1)
	{
		doc();
		Display();
		
		_delay_ms(100);
	}
}