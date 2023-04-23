/*
* C_library.c
*
* Created: 11/02/2018 7:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "uart/uart.h"
#include "i2c/i2c.h"
#include "spi/spi.h"

uint8_t xx[16] = "hello\n";
void rxInt()
{
	UART_println("dasdasd");
	I2C_WriteBuffer((uint8_t*)"hello\n", 6);
}
void receiveFunc()
{
	while (1 < I2C_available()) { // loop through all but the last
		char c = I2C_Read(); // receive byte as a character
		UART_putChar(c);         // print the character
	}
	int x = I2C_Read();    // receive byte as an integer
	UART_printNumln(x, DEC);         // print the integer
}

int main(void)
{
	UART_config(115200);
	UART_println("UART OK!!!");
	I2C_Begin(8);
	I2C_onRequest(rxInt);
	I2C_onReceive(receiveFunc);
	UART_println("I2C OK!!!");
	sei();
	while(1)
	{
		
	}
}
