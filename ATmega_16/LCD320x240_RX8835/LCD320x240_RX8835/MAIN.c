/*
* LCD320x240_Rx8835.c
*
* Created: 08/02/2018 10:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "myLCD_Rx8835.h"

int main(void)
{
	_delay_ms(500);
	Rx8835_Config();// cai dat ban dau cho LCD
	Rx8835_DrawForm();// ve giao dien
	double x = 123, y = 83, z = 564, w = 57, a = 12, b = 55, tempx = 3, tempy = 3, tempz = 3, tempw = 3, tempa = 3, tempb = 3;
	while(1)
	{
		Rx8835_PrintData(x, y, z, w, a, b);
		//Rx8835_PutChar_x29(211, 3, 1);
		x += tempx;
		y += tempy;
		z += tempz;
		w += tempw;
		a += tempa;
		b += tempb;
		
		if (x<20) tempx = 5;
		else if (x>150) tempx = -5;
		if (y<20) tempy = 5;
		else if (y>140) tempy = -5;
		if (z<20) tempz = 5;
		else if (z>120) tempz = -5;
		if (w<20) tempw = 5;
		else if (w>150) tempw = -3;
		if (a<20) tempa = 5;
		else if (a>120) tempa = -5;
		if (b<20) tempb = 5;
		else if (b>150) tempb = -3;
		
		_delay_ms(200);
	}
}
