/*
 * LCD_595.c
 *
 * Created: 3/19/2016 8:02:04 PM
 *  Author: Administrator
 */ 
#include "F:\AVR\Header\mySTART.h"
#include "my595.h"
char ma[]={0X3F,0x06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
int code[] = {0x08,0x28,0x20,0x28,0x20,0x80,0x88,0x80,0x00,0x08,0x00,0xc0,0xc8,0xc0,0x00,0x08,0x00,
	0x60,0x68,0x60,0x80,0x88,0x80,0x88,0x08,0x00,0x64,0x6c,0x64,0x14,0x1c,0x14};
#define sbi(sfr,bit) sfr|=_BV(bit)
#define cbi(sfr,bit) sfr&=~(_BV(bit))
char en,rs;
#define EN 3
#define RS 2

#define LCD_DDR DDRA
#define LCD_PORT PORTA
#define LCD_PIN PINA

#define CTRL LCD_PORT
#define DDR_CTRL LCD_DDR

#define DATA_O LCD_PORT
#define DATA_I LCD_PIN
#define DDR_DATA LCD_DDR
//---------------------------------------------------------------------
void Write2Nib(uint8_t chr)
{
	uint8_t HNib, LNib;//, temp_data;
	//temp_data=DATA_O & 0x0F; //doc 4 bit thap cua DATA_O de mask,

	HNib=chr & 0xF0;
	LNib=(chr<<4) & 0xF0;

	Ship_595((en<<EN)|(rs<<RS)|HNib);
	
	en=1;
	Ship_595((en<<EN)|(rs<<RS)|HNib);
	
	en=0;
	Ship_595((en<<EN)|(rs<<RS)|HNib);
	
	Ship_595((en<<EN)|(rs<<RS)|LNib);
	
	en=1;
	Ship_595((en<<EN)|(rs<<RS)|LNib);
	
	en=0;
	Ship_595((en<<EN)|(rs<<RS)|LNib);
	
}
//-----------------------------------------------------------------------
void wait_LCD()
{
	_delay_ms(1);
}
//-----------------------------------------------------------------------
void init_LCD()
{
	//Function set------------------------------------------------------------------------------
	
	en=rs=0;
	Ship_595((en<<EN)|(rs<<RS));//cbi(CTRL,RS);   // the following data is COMMAND
	//
	en=0;
	Ship_595((en<<EN)|(rs<<RS));//cbi(CTRL, EN);
	//
	en=1;
	Ship_595((en<<EN)|(rs<<RS));//sbi(CTRL,EN); //enable
	
	Ship_595((en<<EN)|(rs<<RS)|(1<<5));//sbi(DATA_O, 5);
	
	en=0;
	Ship_595((en<<EN)|(rs<<RS)|(1<<5));//cbi(CTRL,EN); //disable
	
	wait_LCD();
	Write2Nib(0x28);//4 bit mode, 2 line, 5x8 font
	wait_LCD();
	
	//Display control-------------------------------------------------------------------------
	rs=0;
	Ship_595((en<<EN)|(rs<<RS));
	

	
	Write2Nib(0x0C);
	//Write2Nib(0x0E);-------------co con tro
	wait_LCD();
	
	//Entry mode set------------------------------------------------------------------------
	rs=0;
	Ship_595((en<<EN)|(rs<<RS));//cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x06);
	wait_LCD();
	
}

void home_LCD()
{
	//cbi(CTRL,RS); // the following data is COMMAND
	rs=0;
	Ship_595((en<<EN)|(rs<<RS));
	//_delay_ms(500);
	Write2Nib(0x02);
	wait_LCD();
	
}
void move_LCD(uint8_t y,uint8_t x)
{
	uint8_t Ad;
	Ad=64*(y-1)+(x-1)+0x80; // tính mã l?nh
	//cbi(CTRL,RS); // the following data is COMMAND
	rs=0;
	Ship_595((en<<EN)|(rs<<RS));
	
	Write2Nib(Ad);
	wait_LCD();
	
}


void putChar_LCD(uint8_t chr)
{ //hi?n th? 1 ký t? chr lên LCD
	rs=1;
	Ship_595((en<<EN)|(rs<<RS));
	//sbi(CTRL,RS); //this is a normal DATA
	
	Write2Nib(chr);
	wait_LCD();
	
}
void print_LCD(char* str)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; str[i]!=0; i++) 
	putChar_LCD(str[i]);
}
void vietso_LCD(int num)
{
	char dis[10];
	sprintf(dis, "%d", num);
	print_LCD(dis);
}
void clr_LCD()
{ //xóa toàn b? LCD
	//cbi(CTRL,RS); //RS=0 mean the following data is COMMAND (not normal DATA)
	//Write2Nib(0x01);
	//wait_LCD();
	rs=0;
	Ship_595((en<<EN)|(rs<<RS));
	//_delay_ms(500);
	Write2Nib(0x01);
	wait_LCD();
}
int main(void)
{
	//_delay_ms(1000);
	Init_595();
	init_LCD();
	clr_LCD();
	while(1)
	{
		for (int x = 0;x<100;x++)
		{
			move_LCD(1,1);
			vietso_LCD(x);
			_delay_ms(200);
		}
			//_delay_ms(10000);
	}
}