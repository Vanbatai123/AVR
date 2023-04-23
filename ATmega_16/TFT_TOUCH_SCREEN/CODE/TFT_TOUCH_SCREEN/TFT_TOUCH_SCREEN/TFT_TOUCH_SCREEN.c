/*
 * TFT_TOUCH_SCREEN.c
 *
 * Created: 1/21/2017 11:19:52 AM
 *  Author: Van_BasTai
 */ 




#define MINPRESSURE 10
#define MAXPRESSURE 1000

#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myADC.h"
#include "F:\AVR\Header\myTFT_TOUCH.h"
#include "F:\AVR\Header\myUART.h"

int main(void)
{
	init_ADC();
	init_UART();
    while(1)
    {
        getPoint();
        if (z > MINPRESSURE && z < MAXPRESSURE) 
		{
	        print_UART("X = "); 
			vietso_dec_UART(x);
	        print_UART("\tY = "); 
			vietso_dec_UART(y);
	        print_UART("\tPressure = "); 
			vietso_dec_UART(z);
			newline_UART();
			_delay_ms(100);		
		}
	}
}