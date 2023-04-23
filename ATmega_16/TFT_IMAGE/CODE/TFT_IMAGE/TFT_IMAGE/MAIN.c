/*
 * TFT_IMAGE.c
 *
 * Created: 1/28/2017 10:43:29 PM
 *  Author: Van_BasTai
 */ 


#include "mySTART.h"
#include "my595.h"
#include "myLCD_TFT.h"

#define COLOR_TEXT GREEN


int main(void)
{
	Init_595();
	TFT_Init();
	TFT_SetRotation(LEFT);
	TFT_FillScreen(SCREEN_COLOR);
	TFT_Print(0, 215, "/*", BIG, COLOR_TEXT);
	TFT_Print(0, 190, " * DO AN VI DIEU KHIEN", BIG, COLOR_TEXT);
	TFT_Print(0, 165, " * DE TAI: MAY DO XUNG", BIG, COLOR_TEXT);
	TFT_Print(0, 140, " * SVTH: VAN BA TAI", BIG, COLOR_TEXT);
	TFT_Print(0, 115, " * GVHD: DANG PHUOC VINH", BIG, COLOR_TEXT);
	TFT_Print(0, 90, " * LOP : 13CDT1", BIG, COLOR_TEXT);
	TFT_Print(0, 65, " * NHOM: 13.05B", BIG, COLOR_TEXT);
	TFT_Print(0, 40, " * MSSV: 101130174", BIG, COLOR_TEXT);
	TFT_Print(0, 15, " */", BIG, COLOR_TEXT);
	_delay_ms(5000);
	TFT_FillScreen(SCREEN_COLOR);
	TFT_Print(65, 180, "R-OSCILLOSCOPE", BIG, COLOR_TEXT);
	TFT_Print(75, 145, "LOADING:   %", BIG, COLOR_TEXT);
	TFT_DrawRectangle(9, 109, 310, 127, MAGENTA );
	TFT_DrawRectangle(7, 107, 312, 129, MAGENTA );
	for (int count = 10; count < 310; count++)
	{
		TFT_Number_Dec(175, 145, (count - 10)/3 + 1, BIG, COLOR_TEXT);
		TFT_DrawLineY(count, 110, 126, YELLOW);
	}
	TFT_Print(75, 75, "   DONE!!!   ", BIG, COLOR_TEXT);
	_delay_ms(2000);
	TFT_Print(20, 40, "''LET THE GAME BEGIN''", BIG, COLOR_TEXT);
	_delay_ms(2000);
	while(1)
	{
		
	}
}