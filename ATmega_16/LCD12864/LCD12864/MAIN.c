/*
* LCD12864.c
*
* Created: 1/28/2017 10:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "myUART.h"
#include "myLCD.h"



int main(void)
{
	init_UART();
	print_UART("OKE!!!\r\n");
	
	init_LCD();
	//clr_LCD();
	clr_GLCD();
	
	home_LCD();
	putChar_LCD('a');
	move_LCD(3,3);
	putChar_LCD('b');
	while(1)
	{
//		drawPoint_GLCD(63, 127);
	}
}