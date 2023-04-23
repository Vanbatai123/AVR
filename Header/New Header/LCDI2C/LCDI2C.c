/*
* LCDI2C.c
*
* Created: 12/21/2018 11:30:50 AM
*  Author: Van_BasTai
*/

#include "../include.h"
#include "../i2c/i2c.h"
#include "LCDI2C.h"

uint8_t lcdReg = 0;

void LCDI2C_config()
{
	_delay_ms(300);
	lcdReg = 0;
	I2C_begin(0x00);
	
	LCDI2C_write8(0x00);
	_delay_ms(1);
	LCDI2C_write8(0x04);
	_delay_ms(1);
	LCDI2C_write8(0x00);
	_delay_ms(1);
	LCDI2C_write8(0x04);
	_delay_ms(1);
	LCDI2C_write8(0x00);
	_delay_ms(1);
	LCDI2C_write8(0x04);
	_delay_ms(1);
	LCDI2C_write8(0x00);
	_delay_ms(1);
	
	LCDI2C_write8(0x20);// function set// 4 bit mode, 2 line, 5x8 font
	_delay_ms(1);
	LCDI2C_write8(0x24);
	_delay_ms(1);
	LCDI2C_write8(0x20);
	_delay_ms(1);
	
	LCDI2C_write(CMD, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);// Display control
	_delay_ms(1);
	LCDI2C_write(CMD, LCD_ENTRYMODESET | LCD_ENTRYLEFT);// Enter mode set
	_delay_ms(1);
}
void LCDI2C_write8(uint8_t cmd)
{
	I2C_beginTransmission(LCDI2C_ADDRESS);
	I2C_write(cmd);
	I2C_endTransmission();	
	_delay_us(50);
}
void LCDI2C_write(uint8_t cd, uint8_t cmd)// cmd = 0, data = 1
{
	uint8_t HNib, LNib, temp;
	temp = lcdReg & 0x0F;
	if (cd == 1) temp |= LCDI2C_RS;
	else temp &= ~LCDI2C_RS;
	
	HNib = cmd & 0xF0;
	LNib = (cmd<<4) & 0xF0;
	
	lcdReg = HNib | temp;
	LCDI2C_write8(lcdReg);
	lcdReg |= LCDI2C_EN;
	LCDI2C_write8(lcdReg);
	lcdReg &= ~LCDI2C_EN;
	LCDI2C_write8(lcdReg);

	lcdReg = LNib | temp;
	LCDI2C_write8(lcdReg);
	lcdReg |= LCDI2C_EN;
	LCDI2C_write8(lcdReg);
	lcdReg &= ~LCDI2C_EN;
	LCDI2C_write8(lcdReg);
}
void LCDI2C_wait()
{
	_delay_ms(1);
}
void LCDI2C_home()
{
	LCDI2C_write(CMD, 0x02);
	_delay_ms(2);
}
void LCDI2C_goTo(uint8_t y,uint8_t x)
{
	uint8_t ad;
	ad = 64 * (y-1) + (x-1) + 0x80; // calculate cmd code
	
	LCDI2C_write(CMD, ad);
	_delay_ms(1);
}
void LCDI2C_putC(uint8_t chr)
{
	LCDI2C_write(DATA, chr);
	_delay_ms(1);
}
void LCDI2C_print(char* str)
{
	uint8_t i;
	for (i = 0; str[i] != 0; i++)
	LCDI2C_putC(str[i]);
}
void LCDI2C_printNum(int num)
{
	char dis[10];
	sprintf(dis, "%d", num);
	LCDI2C_print(dis);
}
void LCDI2C_clear()
{
	LCDI2C_write(CMD, 0x01);
	_delay_ms(1);
}
void LCDI2C_blackLight(uint8_t stt)
{
	if (stt) lcdReg |= LCD_BACKLIGHT;
	else lcdReg &= ~LCD_BACKLIGHT;
	LCDI2C_write(CMD, lcdReg);
}