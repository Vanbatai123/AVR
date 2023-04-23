/*
 * TFT.c
 *
 * Created: 1/28/2013 10:34:32 AM
 *  Author: NguyQuyNhat
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "HW_AVR_defines.h"
//#include "memorysaver.h"
#include "UTFT.h"
//#include "stdio.h"
//#include "anh.h"
//#include "var.h"
int main(void)
{
	DDRA=0xff;
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0xff;
	sbi(PORT_RD,BIT_RD);
	cbi(PORT_RD,BIT_RW);
	_delay_ms(100);
	InitLCD(LANDSCAPE);
	_delay_ms(100);
	fillScr2(VGA_BLACK);
	_delay_ms(1000);
	fillScr2(VGA_WHITE);
	_delay_ms(1000);
	fillScr2(VGA_RED);
	_delay_ms(1000);
	fillScr2(VGA_GREEN);
	_delay_ms(1000);
	fillScr2(VGA_BLUE);
	_delay_ms(1000);
	fillScr2(VGA_SILVER);
	_delay_ms(1000);
	fillScr2(VGA_GRAY);
	_delay_ms(1000);
	fillScr2(VGA_MAROON);
	_delay_ms(1000);
	fillScr2(VGA_YELLOW);
	_delay_ms(1000);
	fillScr2(VGA_OLIVE);
	_delay_ms(1000);
	fillScr2(VGA_LIME);
	_delay_ms(1000);
	fillScr2(VGA_AQUA);
	_delay_ms(1000);
	fillScr2(VGA_TEAL);
	_delay_ms(1000);
	fillScr2(VGA_NAVY);
	_delay_ms(1000);
	fillScr2(VGA_FUCHSIA);
	_delay_ms(1000);
	fillScr2(VGA_PURPLE);
	_delay_ms(1000);
	fillScr2(VGA_WHITE);
	_delay_ms(1000);
	setBackColor2(VGA_WHITE);
	
	//drawBitmap1(120,171,80,68,&logo,1);
	setColor2(VGA_RED);
	print1("PHAM MANH TOAN  - manhtoan.bk9@gmail.com ",0,0,0);
	print1("WELCOME TO AVR ",113,130,0);
	print1("VER 1 - DEMO .",10,10,0);
	drawCircle(190,40,40);
	setColor2(VGA_RED);
	fillCircle(40,90,30);
	setColor2(VGA_BLUE);
	drawRect(180,120,100,70);
	setColor2(VGA_OLIVE);
	fillRect(200,70,290,120);
	_delay_ms(500);
	
    while(1)
    {
		
		
		
    }
}