/*
 * DS1307_BY_ME.c
 *
 * Created: 4/20/2017 08:51:57 PM
 *  Author: Van_BasTai
 */ 

#include "mySTART.h"
//#include "myLCD.h"
asd
/*
#define I2C_PORT	PORTD
#define I2C_DDR		DDRD
#define I2C_PIN		PIND
#define SDA			1
#define SCL			0

#define write_data	setb(I2C_DDR, SDA)
#define read_data	clrb(I2C_DDR, SDA)

char count, second;
int dataa;

void i2c_start()
{
	write_data;
	I2C_PORT|=(1<<SDA)|(1<<SCL);
	clrb(I2C_PORT, SDA);
	_delay_us(10);
	
}
void i2c_stop()
{
	write_data;
	I2C_PORT|=(0<<SDA)|(1<<SCL);
	_delay_us(10);
	setb(I2C_PORT, SDA);
}
void i2c_write1byte(int data)
{
	write_data;
	clrb(I2C_PORT, SCL);
	for (count = 0; count < 8; count++)
	{
		if ((data & 0x80) == 0x80)
			setb(I2C_PORT, SDA);
		else
			clrb(I2C_PORT, SDA);
		setb(I2C_PORT, SCL);
		_delay_us(10);
		clrb(I2C_PORT, SCL);
		data = data<<1;
	}
}
int i2c_read1byte()
{
	read_data;
	I2C_PORT|=(0<<SCL);
	for (count = 0; count < 8; count++)
	{
		setb(I2C_PORT, SCL);
		if (inbit(I2C_PIN, SDA))
			dataa = (dataa<<1) + 1;
		else
			dataa = dataa<<1;
		_delay_us(10);
		clrb(I2C_PORT, SCL);
	}
	return dataa;
}
void i2c_read_ack()
{
	read_data;
	setb(I2C_PORT, SCL);
	_delay_us(10);
	clrb(I2C_PORT, SCL);
}
void i2c_write_ack()
{
	write_data;
	clrb(I2C_PORT, SDA);
	setb(I2C_PORT, SCL);
	_delay_us(10);
	clrb(I2C_PORT, SCL);
}
void i2c_write_notack()
{
	write_data;
	setb(I2C_PORT, SDA);
	setb(I2C_PORT, SCL);
	_delay_us(10);
	clrb(I2C_PORT, SCL);
}
void i2c_WRbit(char wr)
{
	if (wr)
		setb(I2C_PORT, SDA);
	else
		clrb(I2C_PORT, SDA);
	setb(I2C_PORT, SCL);
	_delay_us(10);
	clrb(I2C_PORT, SCL);
}
void i2c_read()
{
	i2c_start();
	i2c_write1byte(0xD0);
	i2c_WRbit(0);
	i2c_read_ack();
	i2c_write1byte(0x01);
	i2c_start();
	i2c_write1byte(0xD0);
	i2c_WRbit(1);
	i2c_read_ack();
	second = 234-123;
	i2c_write_ack();
	i2c_stop();
	
}*/
int main(void)
{
	//I2C_PORT|=(0<<SDA)|(0<<SCL);
	//I2C_DDR|=(1<<SDA)|(1<<SCL); 
	//init_LCD();
	while(1)
	{
		//i2c_read();
		
	}
}