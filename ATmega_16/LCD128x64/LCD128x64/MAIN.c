/*
* LCD128x64.c
*
* Created: 08/01/2018 08:43:29 PM
*  Author: Van_BasTai
*/


#include "include.h"
#include "LCD_HP.h"

#define PORT_DATA	PORTA
#define DDR_DATA	DDRA
#define PIN_DATA	PINA

#define PORT_CTRL	PORTD
#define DDR_CTRL	DDRD

// Pin definition
#define RS			0
#define RW			1
#define EN			2

#define LOW_EN	clrb(PORT_CTRL, EN)
#define HIGH_EN	setb(PORT_CTRL, EN)

#define BUSY_FLAG	0x80

void Config();
uint8_t ReadBusyFlag();
void WriteData(uint8_t dataa);
void WriteCommand(uint8_t cmd);
void Clear(void);
void putChar(unsigned char X, unsigned char Y, unsigned char DData);
/************************************************************************/
/* Main Function                                                        */
/************************************************************************/
int main(void)
{
	_delay_ms(500);
	LCDHP_Config();
	_delay_ms(100);
	LCDHP_Move(1,1);
	LCDHP_Puts("LCD OKE!!!");
	_delay_ms(50);
	Config();
	int cnt = 0;
	_delay_ms(400);				// LUU Y SU CAN THIET CUA KHOANG THOI GIAN DELAY NAY (>40ms)
	Config();
	_delay_ms(5);
	Clear();
	while(1)
	{
		putChar(1,1,0xFF);
		putChar(1,5,0xFF);
		putChar(1,3,0xFF);
		putChar(1,2,0xFF);
		putChar(8,1,0xFF);
		putChar(1,12,0xFF);
	
		
		
		LCDHP_Move(2, 1);
		LCDHP_PrintD(cnt++);
		_delay_ms(700);
	}
}

/************************************************************************/
/* Configuration Function                                               */
/************************************************************************/
void Config()
{
	DDR_DATA = DDR_CTRL = 0xFF;
	LOW_EN;
	
	WriteCommand(0x30); 					// Chon che do truyen 8 bit
	_delay_us(150);											// delay > 100us
	WriteCommand(0x30); 					// Chon che do truyen 8 bit
	_delay_us(50);												// delay > 37us
	
	// Display ON/OFF control
	WriteCommand(0x0C); 					// Cho phep hien thi ra LCD 128x64
	_delay_us(120);											// delay > 100us
	
	// Clear LCD
	WriteCommand(0x01); 					// Xoa man hinh
	_delay_ms(40);												// delay > 10ms
	
	// Entry mode set
	WriteCommand(0x06); 					// Chon che do hien thi ki tu ra man hinh LCD 128x64 (Ki tu sau xuat hien ben phai ki tu truoc)
}

/************************************************************************/
/* Read Busy Flag Function                                              */
/************************************************************************/
uint8_t ReadBusyFlag()
{
	uint8_t flag;
	PORT_DATA = 0xFF;
	DDR_DATA = 0x00;
	_delay_ms(1);
	clrb(PORT_CTRL, RS);
	setb(PORT_CTRL, RW);
	//_delay_ms(10);
	
	HIGH_EN;
	_delay_ms(1);
	flag = PIN_DATA;
	LOW_EN;
	DDR_DATA = 0xFF;
	if ((flag & BUSY_FLAG) == 0x80)
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}

/************************************************************************/
/* Write Data Function                                                  */
/************************************************************************/
void WriteData(uint8_t dataa)
{
	//while(ReadBusyFlag() == 1);
	_delay_ms(1);
	clrb(PORT_CTRL, RW);
	setb(PORT_CTRL, RS);
	//_delay_ms(10);
	
	PORT_DATA = dataa;
	HIGH_EN;
	_delay_us(1);
	LOW_EN;
}
/************************************************************************/
/* Write Command Function                                                  */
/************************************************************************/
void WriteCommand(uint8_t cmd)
{
	//while(ReadBusyFlag() == 1);
	_delay_ms(1);
	clrb(PORT_CTRL, RW);
	clrb(PORT_CTRL, RS);
	//_delay_ms(10);
	PORT_DATA = cmd;
	
	HIGH_EN;
	_delay_us(1);
	LOW_EN;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void Clear()
{
	WriteCommand(0x01);
	//write_command_lcd(0x34,1);
	WriteCommand(0x30);
}

void putChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	if(Y<1)
	Y=1;
	if(Y>4)
	Y=4;
	X &= 0x0F;
	switch(Y)
	{
		case 1:X|=0X80;break;
		case 2:X|=0X90;break;
		case 3:X|=0X88;break;
		case 4:X|=0X98;break;
	}
	WriteCommand(X);
	WriteData(DData);
}