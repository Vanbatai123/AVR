/*
 * DS1307.c
 *
 * Created: 12/24/2018 8:44:16 AM
 *  Author: Van_BasTai
 */ 

#include "../include.h"
#include "DS1307.h"
#include "../i2c/i2c.h"

/* Convert BCD to Decimal */
uint8_t bcd2dec(uint8_t bcd)
{
	return ((bcd/16 * 10) + (bcd % 16));
}
/* Convert Decimal to BCD */
uint8_t dec2bcd(uint8_t dec)
{
	return ((dec/10 * 16) + (dec % 10));
}

void DS1307_read(uint8_t data[])
{
	I2C_beginTransmission(DS1307_ADDRESS);
	I2C_write(0x00);
	I2C_endTransmission();
	I2C_requestFrom(DS1307_ADDRESS, 7);
	
	data[0] = bcd2dec(I2C_read() & 0x7f);
	data[1] = bcd2dec(I2C_read());
	data[2] = bcd2dec(I2C_read() & 0x3f); // mode 24h
	data[3] = bcd2dec(I2C_read());
	data[4] = bcd2dec(I2C_read());
	data[5] = bcd2dec(I2C_read());
	data[6] = bcd2dec(I2C_read());
}

void DS1307_write(uint8_t data[])
{
	I2C_beginTransmission(DS1307_ADDRESS);
	I2C_write(0x00); // set pointer to zero
	I2C_write(dec2bcd(data[0]));
	I2C_write(dec2bcd(data[1]));
	I2C_write(dec2bcd(data[2]));
	I2C_write(dec2bcd(data[3])); // day of week: Sunday = 1, Saturday = 7
	I2C_write(dec2bcd(data[4]));
	I2C_write(dec2bcd(data[5]));
	I2C_write(dec2bcd(data[6]));
	I2C_endTransmission();
}