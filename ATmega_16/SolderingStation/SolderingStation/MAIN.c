/*
 * SolderingStation.c
 *
 * Created: 10/27/2017 05:34:57 PM
 *  Author: Van_BasTai
 */ 


#include "mySTART.h"
#include "myLCD_HP.h"
#include "myADC.h"



int main(void)
{
	init_ADC();
	init_LCD();
	move_LCD(1,1);
	print_LCD("TEST LCD");
	while(1)
	{
		long adc=read_ADC(0);
		int r=(float)((102300-100*adc)/adc);
		float t=(float)(4.54*r-213.64);
		move_LCD(2,1);
		vietso_dec_LCD(r);
		print_LCD("    ");
		move_LCD(3,1);
		vietso_dec_LCD(t);
		print_LCD("      ");
	}
}