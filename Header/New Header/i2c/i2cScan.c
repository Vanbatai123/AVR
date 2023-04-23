/*
* C_library.c
*
* Created: 11/02/2018 7:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "uart/uart.h"
#include "i2c/i2c.h"

int main(void)
{
	UART_config(115200);
	UART_println("UART OK!!!");
	I2C_begin(0x00);
	UART_println("I2C OK!!!");
	_delay_ms(200);
	while(1)
	{
		uint8_t error, address;
		int nDevices;
		UART_println("Scanning...");
		nDevices = 0;
		//UART_printNumln(error, HEX);
		for(address = 0x30; address < 127; address++ )
		{
			error = I2C_beginTransmission(address);
			I2C_endTransmission();
			if (error == 0)
			{
				UART_print("I2C device found at address ");
				if (address<16)
				UART_print("0");
				UART_printNum(address,HEX);
				UART_println("  !");
				nDevices++;
			}
		}
		if (nDevices == 0)
		UART_println("No I2C devices found\n");
		else
		UART_println("done\n");
		_delay_ms(2000);           // wait 5 seconds for next scan
	}
}
