/*
 * _24Cxx.c
 *
 * Created: 12/24/2018 1:22:13 PM
 *  Author: Van_BasTai
 */ 
#include "../include.h"
#include "E24Cxx.h"
#include "../i2c/i2c.h"

void E24C_config()
{
	I2C_begin(0x00);
}

void E24C_write(unsigned int addr, uint8_t data)
{
	I2C_beginTransmission(E24CXX_ADDRESS);
	I2C_write((int)(addr >> 8));   // MSB
	I2C_write((int)(addr & 0xFF)); // LSB
	I2C_write(data);                  // Write uint8_t
	I2C_endTransmission();

	// Writing in I2C EEPROM takes ~5ms (even if I2C writing already done)
	_delay_ms(5);
}

uint8_t E24C_read(unsigned int addr)
{
	uint8_t read_data = 0xFF;

	I2C_beginTransmission(E24CXX_ADDRESS);
	I2C_write((int)(addr >> 8));   // MSB
	I2C_write((int)(addr & 0xFF)); // LSB
	I2C_endTransmission();

	// Request 1 uint8_t from device
	I2C_requestFrom(E24CXX_ADDRESS, 1);

	if (I2C_available()) {
		read_data = I2C_read();
	}

	return read_data;
}

