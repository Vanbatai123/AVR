/*
 * LCD_TFT_2.c
 *
 * Created: 1/7/2017 10:34:32 AM
 *  Author: Van_BasTai
 */ 

#include "mySTART.h"
#include "myLCD_TFT.h"

int main(void)
{
	TFT_Init();
	TFT_FillScreen(SCREEN_COLOR);
	TFT_SetRotation(TOP);
	
    while(1)
    {
		
    }
}
