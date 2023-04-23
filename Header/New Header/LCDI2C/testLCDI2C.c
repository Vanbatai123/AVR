/*
* C_library.c
*
* Created: 11/02/2018 7:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "uart/uart.h"
#include "i2c/i2c.h"
#include "LCDI2C/LCDI2C.h"

int main(void)
{
	//UART_config(115200);
	//UART_println("UART OK!!!");
	//I2C_begin(0x00);
	//UART_println("I2C OK!!!");
	_delay_ms(300);
	I2C_begin(0x00);
	LCDI2C_config();
	LCDI2C_blackLight(0);
	LCDI2C_clear();
	while(1)
	{
		LCDI2C_GoTo(1, 1);
		LCDI2C_Print("asdfghjklqweyu");
		_delay_ms(1000);
	}
}
