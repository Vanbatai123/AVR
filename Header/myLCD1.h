/*
 * Z1.c
 *
 * Created: 3/23/2015 9:28:08 AM
 *  Author: HUU_QUOC
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define sbi(sfr,bit) sfr|=_BV(bit)
#define cbi(sfr,bit) sfr&=~(_BV(bit))

#define EN 3
#define RS 2

#define CTRL PORTD
#define DDR_CTRL DDRD

#define DATA_O PORTD
#define DATA_I PIND
#define DDR_DATA DDRD
char Read2Nib()
{
	char HNib, LNib;
	DATA_O|=0xF0;

	sbi(CTRL,EN); //enable
	DDR_DATA &=0x0F; //set 4 bits cao cua PORT DATA lam input
	HNib=DATA_I & 0xF0;
	cbi(CTRL,EN); //disable

	sbi(CTRL,EN); //enable
	LNib = DATA_I & 0xF0;
	cbi(CTRL,EN); //disable
	LNib>>=4;
	return (HNib|LNib);
}

void Write2Nib(uint8_t chr)
{
	uint8_t HNib, LNib, temp_data;
	temp_data=DATA_O & 0x0F; //doc 4 bit thap cua DATA_O de mask,

	HNib=chr & 0xF0;
	LNib=(chr<<4) & 0xF0;

	DATA_O =(HNib |temp_data);
	sbi(CTRL,EN); //enable
	cbi(CTRL,EN); //disable

	DATA_O =(LNib|temp_data);
	sbi(CTRL,EN); //enable
	cbi(CTRL,EN); //disable
}

void wait_LCD()
{
	_delay_us(1);
}

void init_LCD()
{
	DDR_CTRL=0xFF;
	sbi(DDR_DATA,4);
	sbi(DDR_DATA,5);
	sbi(DDR_DATA,6);
	sbi(DDR_DATA,7);
	//Function set------------------------------------------------------------------------------
	cbi(CTRL,RS);   // the following data is COMMAND
	
	cbi(CTRL, EN);
	
	sbi(CTRL,EN); //enable
	sbi(DATA_O, 5);
	cbi(CTRL,EN); //disable
	wait_LCD();
	Write2Nib(0x28);//4 bit mode, 2 line, 5x8 font
	wait_LCD();
	
	//Display control-------------------------------------------------------------------------
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x0C);
	wait_LCD();
	
	//Entry mode set------------------------------------------------------------------------
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x06);
	wait_LCD();
	
}

void home_LCD()
{
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x02);
	wait_LCD();
	
}
void move_LCD(uint8_t y,uint8_t x)
{
	uint8_t Ad;
	Ad=64*(y-1)+(x-1)+0x80; // tính mã l?nh
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(Ad);
	wait_LCD();
	
}

void clr_LCD()
{ //xóa toàn b? LCD
	cbi(CTRL,RS); //RS=0 mean the following data is COMMAND (not normal DATA)
	
	Write2Nib(0x01);
	wait_LCD();
	
}
void putChar_LCD(uint8_t chr)
{ //hi?n th? 1 ký t? chr lên LCD
	sbi(CTRL,RS); //this is a normal DATA
	
	Write2Nib(chr);
	wait_LCD();
	
}
void print_LCD(char* str)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; str[i]!=0; i++) 
	putChar_LCD(str[i]);
}