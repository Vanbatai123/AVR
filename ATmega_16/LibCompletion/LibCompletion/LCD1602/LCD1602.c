/*
 * LCD1602.c
 *
 * Created: 8/31/2018 2:06:15 PM
 *  Author: ASUS PC
 */ 

#include "../include.h"
#include "LCD1602.h"

char Read2Nib()
{
	char HNib, LNib;
	LCD1602_PORT|=0xF0;

	setb(LCD1602_PORT, LCD1602_PIN_EN); //enable
	LCD1602_DDR &=0x0F; //set 4 bits cao cua PORT DATA lam input
	HNib=LCD1602_PIN & 0xF0;
	clrb(LCD1602_PORT, LCD1602_PIN_EN); //disable

	setb(LCD1602_PORT,LCD1602_PIN_EN); //enable
	LNib = LCD1602_PIN & 0xF0;
	clrb(LCD1602_PORT, LCD1602_PIN_EN); //disable
	LNib>>=4;
	return (HNib|LNib);
}

void Write2Nib(uint8_t chr)
{
	uint8_t HNib, LNib, temp_data;
	temp_data=LCD1602_PORT & 0x0F; //read 4 low-bit

	HNib=chr & 0xF0;
	LNib=(chr<<4) & 0xF0;

	LCD1602_PORT =(HNib |temp_data);
	setb(LCD1602_PORT,LCD1602_PIN_EN); //enable
	clrb(LCD1602_PORT,LCD1602_PIN_EN); //disable

	LCD1602_PORT =(LNib|temp_data);
	setb(LCD1602_PORT,LCD1602_PIN_EN); //enable
	clrb(LCD1602_PORT,LCD1602_PIN_EN); //disable
}

void wait_LCD()
{
	_delay_ms(1);
}

void LCD1602_Config()
{
	LCD1602_DDR = 0xFF;
	setb(LCD1602_DDR,4);
	setb(LCD1602_DDR,5);
	setb(LCD1602_DDR,6);
	setb(LCD1602_DDR,7);
	//Function set------------------------------------------------------------------------------
	clrb(LCD1602_PORT,LCD1602_PIN_RS);   // the following data is COMMAND
	
	clrb(LCD1602_PORT, LCD1602_PIN_EN);
	
	setb(LCD1602_PORT,LCD1602_PIN_EN); //enable
	setb(LCD1602_PORT, 5);
	clrb(LCD1602_PORT,LCD1602_PIN_EN); //disable
	wait_LCD();
	Write2Nib(0x28);//4 bit mode, 2 line, 5x8 font
	wait_LCD();
	
	//Display control-------------------------------------------------------------------------
	clrb(LCD1602_PORT,LCD1602_PIN_RS); // the following data is COMMAND
	
	Write2Nib(0x0C);
	//Write2Nib(0x0E);-------------co con tro
	wait_LCD();
	
	//Entry mode set------------------------------------------------------------------------
	clrb(LCD1602_PORT,LCD1602_PIN_RS); // the following data is COMMAND
	
	Write2Nib(0x06);
	wait_LCD();
	
}

void LCD1602_Home()
{
	clrb(LCD1602_PORT,LCD1602_PIN_RS); // the following data is COMMAND
	
	Write2Nib(0x02);
	wait_LCD();
	
}

void LCD1602_Move(uint8_t y,uint8_t x)
{
	uint8_t Ad;
	Ad=64*(y-1)+(x-1)+0x80; // calculate the command code
	clrb(LCD1602_PORT,LCD1602_PIN_RS); // the following data is COMMAND
	
	Write2Nib(Ad);
	wait_LCD();
	
}

void LCD1602_PutC(uint8_t chr)
{
	setb(LCD1602_PORT,LCD1602_PIN_RS); //this is a normal DATA
	
	Write2Nib(chr);
	wait_LCD();
	
}

void LCD1602_PutS(char* str)
{
	while(*str) LCD1602_PutC(*str++);
}

void LCD1602_PutNum(double num, uint8_t digit)
{
	char *str = "";
	switch (digit)
	{
		case BIN:
		break;
		
		case DEC:
		sprintf(str, "%ld", (int32_t)num);
		break;
		
		case HEX:
		sprintf(str, "0x%lX", (uint32_t)num);
		break;
		default:
		break;
	}
	LCD1602_PutS(str);
}

void LCD1602_PutF(double number, uint8_t digits)
{
	// Handle negative numbers
	if (number < 0.0)
	{
		LCD1602_PutC('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;
	
	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	LCD1602_PutNum(int_part, DEC);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) 
	{
		LCD1602_PutC('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		LCD1602_PutNum(toPrint, DEC);
		remainder -= toPrint;
	}
}

void LCD1602_Clear()
{ 
	LCD1602_Move(1,1);
	LCD1602_PutS("                ");
	LCD1602_Move(2,1);
	LCD1602_PutS("                ");
	LCD1602_Move(1,1);
}