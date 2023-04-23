/*
* LCD192x64.c
*
* Created: 8/11/2018 10:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "myUART.h"
#include "KS0108/KS0108.h"

int main(void)
{
	init_UART();
	Ks0108_Config();
	Ks0108_WriteCommand(DISPLAY_ON);
	print_UART("OKE!!!\r\n");
	while(1)
	{
		vietso_hex_UART(Ks0108_ReadStatus());
		putchar_UART('\r');
		_delay_ms(600);

		Ks0108_WriteCommand(START_LINE + 0);
		Ks0108_WriteCommand(SET_Y_ADDR + 5);
		Ks0108_WriteCommand(SET_X_ADDR + 0);
		Ks0108_WriteData(0x0F);
	}
}