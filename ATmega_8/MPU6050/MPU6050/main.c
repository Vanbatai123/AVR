/*
* MPU6050.c
*
* Created: 2/27/2019 8:55:42 PM
* Author : PC
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
		I2C_beginTransmission(0x68);
		I2C_write(0x31);
		I2C_endTransmission();
		I2C_requestFrom(0x68, 1);
		UART_printNumln(I2C_read(), HEX);
		_delay_ms(1000);
	}
}

