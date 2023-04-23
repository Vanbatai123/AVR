/*
  UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2012 Henning Karlsen. All right reserved
  
  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of 
  supported display modules and controllers started to increase I felt 
  it was time to make a single, universal library as it will be much 
  easier to maintain in the future.

  Basic functionality of this library was origianlly based on the 
  demo-code provided by ITead studio (for the ITDB02 modules) and 
  NKC Electronics (for the RGB GLCD module/shield).

  This library supports a number of 8bit, 16bit and serial graphic 
  displays, and will work with both Arduino and chipKit boards. For a 
  full list of tested display modules and controllers, see the 
  document UTFT_Supported_display_modules_&_controllers.pdf.

  When using 8bit and 16bit display modules there are some 
  requirements you must adhere to. These requirements can be found 
  in the document UTFT_Requirements.pdf.
  There are no special requirements when using serial displays.

  You can always find the latest version of the library at 
  http://electronics.henningkarlsen.com/

  If you make any modifications or improvements to the code, I would 
  appreciate that you share the code with me so that I might include 
  it in the next release. I can be contacted through 
  http://electronics.henningkarlsen.com/contact.php.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "UTFT.h"
#include <pins_arduino.h>

// Include hardware-specific functions for the correct MCU
#if defined(__AVR__)
	#include <avr/pgmspace.h>
	#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		#include "HW_ATmega1280.h" 
	#elif defined(__AVR_ATmega328P__)
		#include "HW_ATMega328P.h"
	#elif defined(__AVR_ATmega32U4__)
		#include "HW_ATMega32U4.h"
	#elif defined(__AVR_ATmega168__)
		#error "ATmega168 MCUs are not supported because they have too little flash memory!"
	#else
		#error "Unsupported AVR MCU!"
	#endif
#elif defined(__PIC32MX__)
  #if defined(__32MX320F128H__)
    #pragma message("Compiling for chipKIT UNO32 (__32MX320F128H__)")
	#include "HW_PIC32MX320F128H.h"
  #elif defined(__32MX340F512H__)
    #error "chipKIT uC32 (__32MX340F512H__) is not supported."
  #elif defined(__32MX795F512L__)
    #pragma message("Compiling for chipKIT MAX32 (__32MX795F512L__)")
	#include "HW_PIC32MX795F512L.h"
  #else
    #error "Unsupported PIC32 MCU!"
  #endif  
#elif defined(__arm__)
	#if defined(__SAM3X8E__)
		#pragma message("Compiling for Arduino Due (AT91SAM3X8E)...")
		#include "HW_SAM3X8E.h"
	#else
		#error "Unsupported ARM MCU!"
	#endif
#endif
#include "memorysaver.h"
void LCD_Writ_Bus(char VH,char VL, int mode)
{   
	switch (mode)
	{
	case 1:
		if (display_serial_mode==SERIAL_4PIN)
		{
		if (VH==1)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		}
		else
		{
		if (VH==1)
			sbi(PORT_RS, BIT_RS);
		else
			cbi(PORT_RS, BIT_RS);
		}

		if (VL & 0x80)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x40)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x20)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x10)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x08)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x04)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x02)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		if (VL & 0x01)
			sbi(PORT_SDA, BIT_SDA);
		else
			cbi(PORT_SDA, BIT_SDA);
		pulse_low(PORT_CSL, BIT_SCL);
		break;
	case 8:
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		PORTA = VH;
		pulse_low(PORT_RW, BIT_RW);
		PORTA = VL;
		pulse_low(PORT_RW, BIT_RW);
#else
		PORTD = VH;
		pulse_low(PORT_RW, BIT_RW);
		PORTD = VL;
		pulse_low(PORT_RW, BIT_RW);
#endif
		break;
	case 16:
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		PORTA = VH;
		PORTC = VL;
#else
		PORTD = VH;
		cport(PORTC, 0xFC);
		sport(PORTC, (VL>>6) & 0x03);
		PORTB =  VL & 0x3F;
#endif
		pulse_low(PORT_RW, BIT_RW);
		break;
	case LATCHED_16:
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		PORTG &= ~0x20;	
		PORTG |= (VH & 0x10)<<1;	
		PORTH &= ~0x18;	
		PORTH |= (VH & 0xC0)>>3;	
		PORTE &= ~0x3B;	
		PORTE |= (VH & 0x03) + ((VH & 0x0C)<<2) + ((VH & 0x20)>>2);
		cbi(P_ALE, B_ALE);
		pulse_high(P_ALE, B_ALE);
		cbi(PORT_CS, BIT_CS);
		PORTG &= ~0x20;	
		PORTG |= (VL & 0x10)<<1;	
		PORTH &= ~0x18;	
		PORTH |= (VL & 0xC0)>>3;	
		PORTE &= ~0x3B;	
		PORTE |= (VL & 0x03) + ((VL & 0x0C)<<2) + ((VL & 0x20)>>2);
#else
		PORTD = VH;
		cbi(P_ALE, B_ALE);
		pulse_high(P_ALE, B_ALE);
		cbi(PORT_CS, BIT_CS);
		PORTD =  VL;
#endif
		pulse_low(PORT_RW, BIT_RW);
		sbi(PORT_CS, BIT_CS);
		break;
	}
}

void _set_direction_registers(byte mode)
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (mode!=LATCHED_16)
	{
		DDRA = 0xFF;
		if (mode==16)
			DDRC = 0xFF;
	}
	else
	{
		DDRH = 0x18;
		DDRG = 0x20;
		DDRE = 0x3B;
	}
#else
	DDRD = 0xFF;
	if (mode==16)
	{
		DDRB |= 0x3F;
		DDRC |= 0x03;
	}
#endif

}

void _fast_fill_16(int ch, int cl, long pix)
{
	long blocks;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	PORTA = ch;
	PORTC = cl;
#else
	PORTD = ch;
	cport(PORTC, 0xFC);
	sport(PORTC, (cl>>6) & 0x03);
	PORTB =  cl & 0x3F;
#endif

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(PORT_RW, BIT_RW);
		}
}

void _fast_fill_8(int ch, long pix)
{
	long blocks;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	PORTA = ch;
#else
	PORTD = ch;
#endif

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(PORT_RW, BIT_RW);pulse_low(PORT_RW, BIT_RW);
		}
}
UTFT(byte model, int RS, int WR,int CS, int RST, int SER)
{ 
	switch (model)
	{
		case HX8347A:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=16;
			break;
		case ILI9327:
			disp_x_size=239;
			disp_y_size=399;
			display_transfer_mode=16;
			break;
		case SSD1289:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=16;
			break;
		case ILI9325C:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=8;
			break;
		case ILI9325D_8:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=8;
			break;
		case ILI9325D_16:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=16;
			break;
		case HX8340B_8:
			disp_x_size=175;
			disp_y_size=219;
			display_transfer_mode=8;
			break;
		case HX8340B_S:
			disp_x_size=175;
			disp_y_size=219;
			display_transfer_mode=1;
			display_serial_mode=SERIAL_4PIN;
			break;
		case HX8352A:
			disp_x_size=239;
			disp_y_size=399;
			display_transfer_mode=16;
			break;
		case ST7735:
			disp_x_size=127;
			disp_y_size=159;
			display_transfer_mode=1;
			display_serial_mode=SERIAL_5PIN;
			break;
		case PCF8833:
			disp_x_size=127;
			disp_y_size=127;
			display_transfer_mode=1;
			display_serial_mode=SERIAL_5PIN;
			break;
		case S1D19122:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=16;
			break;
		case SSD1963_480:
			disp_x_size=271;
			disp_y_size=479;
			display_transfer_mode=16;
			break;
		case SSD1963_800:
			disp_x_size=479;
			disp_y_size=799;
			display_transfer_mode=16;
			break;
		case S6D1121_8:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=8;
			break;
		case S6D1121_16:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=16;
			break;
		case SSD1289LATCHED:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=LATCHED_16;
			break;
		case ILI9320_8:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=8;
			break;
		case ILI9320_16:
			disp_x_size=239;
			disp_y_size=319;
			display_transfer_mode=16;
			break;
	}
	display_model=model;

	if (display_transfer_mode!=1)
	{
		_set_direction_registers(display_transfer_mode);
		PORT_RS	= portOutputRegister(digitalPinToPort(RS));
		BIT_RS	= digitalPinToBitMask(RS);
		PORT_RW	= portOutputRegister(digitalPinToPort(WR));
		BIT_RW	= digitalPinToBitMask(WR);
		PORT_CS	= portOutputRegister(digitalPinToPort(CS));
		BIT_CS	= digitalPinToBitMask(CS);
		PORT_RST	= portOutputRegister(digitalPinToPort(RST));
		BIT_RST	= digitalPinToBitMask(RST);
		if (display_transfer_mode==LATCHED_16)
		{
			P_ALE	= portOutputRegister(digitalPinToPort(SER));
			B_ALE	= digitalPinToBitMask(SER);
			pinMode(SER,OUTPUT);
			cbi(P_ALE, B_ALE);
			pinMode(8,OUTPUT);
			digitalWrite(8, LOW);
		}
		pinMode(RS,OUTPUT);
		pinMode(WR,OUTPUT);
		pinMode(CS,OUTPUT);
		pinMode(RST,OUTPUT);
	}
	else
	{
		PORT_SDA	= portOutputRegister(digitalPinToPort(RS));
		BIT_SDA	= digitalPinToBitMask(RS);
		PORT_CSL	= portOutputRegister(digitalPinToPort(WR));
		BIT_SCL	= digitalPinToBitMask(WR);
		PORT_CS	= portOutputRegister(digitalPinToPort(CS));
		BIT_CS	= digitalPinToBitMask(CS);
		PORT_RST	= portOutputRegister(digitalPinToPort(RST));
		BIT_RST	= digitalPinToBitMask(RST);
		if (display_serial_mode!=SERIAL_4PIN)
		{
			PORT_RS	= portOutputRegister(digitalPinToPort(SER));
			BIT_RS	= digitalPinToBitMask(SER);
			pinMode(SER,OUTPUT);
		}
		pinMode(RS,OUTPUT);
		pinMode(WR,OUTPUT);
		pinMode(CS,OUTPUT);
		pinMode(RST,OUTPUT);
	}
}

void LCD_Write_COM(char VL)  
{   
	if (display_transfer_mode!=1)
	{
		cbi(PORT_RS, BIT_RS);
		LCD_Writ_Bus(0x00,VL,display_transfer_mode);
	}
	else
		LCD_Writ_Bus(0x00,VL,display_transfer_mode);
}

void LCD_Write_DATA(char VH,char VL)
{
	if (display_transfer_mode!=1)
	{
		sbi(PORT_RS, BIT_RS);
		LCD_Writ_Bus(VH,VL,display_transfer_mode);
	}
	else
	{
		LCD_Writ_Bus(0x01,VH,display_transfer_mode);
		LCD_Writ_Bus(0x01,VL,display_transfer_mode);
	}
}

void LCD_Write_DATA(char VL)
{
	if (display_transfer_mode!=1)
	{
		sbi(PORT_RS, BIT_RS);
		LCD_Writ_Bus(0x00,VL,display_transfer_mode);
	}
	else
		LCD_Writ_Bus(0x01,VL,display_transfer_mode);
}

void LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8,dat1);
}

void InitLCD(byte orientation)
{
	orient=orientation;
	_hw_special_init();

	sbi(PORT_RST, BIT_RST);
	delay(5); 
	cbi(PORT_RST, BIT_RST);
	delay(15);
	sbi(PORT_RST, BIT_RST);
	delay(15);
	cbi(PORT_CS, BIT_CS);
	switch(display_model)
	{
#ifndef DISABLE_HX8347A
	#include "tft_drivers/hx8347a/initlcd.h"
#endif
#ifndef DISABLE_ILI9327
	#include "tft_drivers/ili9327/initlcd.h"
#endif
#ifndef DISABLE_SSD1289
	#include "tft_drivers/ssd1289/initlcd.h"
#endif
#ifndef DISABLE_ILI9325C
	#include "tft_drivers/ili9325c/initlcd.h"
#endif
#ifndef DISABLE_ILI9325D
	#include "tft_drivers/ili9325d/initlcd.h"
#endif
#ifndef DISABLE_HX8340B_8
	#include "tft_drivers/hx8340b/8/initlcd.h"
#endif
#ifndef DISABLE_HX8340B_S
	#include "tft_drivers/hx8340b/s/initlcd.h"
#endif
#ifndef DISABLE_ST7735
	#include "tft_drivers/st7735/initlcd.h"
#endif
#ifndef DISABLE_PCF8833
	#include "tft_drivers/pcf8833/initlcd.h"
#endif
#ifndef DISABLE_S1D19122
	#include "tft_drivers/s1d19122/initlcd.h"
#endif
#ifndef DISABLE_HX8352A
	#include "tft_drivers/hx8352a/initlcd.h"
#endif
#ifndef DISABLE_SSD1963_480
	#include "tft_drivers/ssd1963/480/initlcd.h"
#endif
#ifndef DISABLE_SSD1963_800
	#include "tft_drivers/ssd1963/800/initlcd.h"
#endif
#ifndef DISABLE_S6D1121
	#include "tft_drivers/s6d1121/initlcd.h"
#endif
#ifndef DISABLE_ILI9320
	#include "tft_drivers/ili9320/initlcd.h"
#endif
	}
	sbi (PORT_CS, BIT_CS); 
	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	cfont.font=0;
}

void setXY(word x1, word y1, word x2, word y2)
{
	int tmp;

	if (orient==LANDSCAPE)
	{
		swap(word, x1, y1);
		swap(word, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(word, y1, y2)
	}

	switch(display_model)
	{
#ifndef DISABLE_HX8347A
	#include "tft_drivers/hx8347a/setxy.h"
#endif
#ifndef DISABLE_HX8352A
	#include "tft_drivers/hx8352a/setxy.h"
#endif
#ifndef DISABLE_ILI9327
	#include "tft_drivers/ili9327/setxy.h"
#endif
#ifndef DISABLE_SSD1289
	#include "tft_drivers/ssd1289/setxy.h"
#endif
#ifndef DISABLE_ILI9325C
	#include "tft_drivers/ili9325c/setxy.h"
#endif
#ifndef DISABLE_ILI9325D
	#include "tft_drivers/ili9325d/setxy.h"
#endif
#ifndef DISABLE_HX8340B_8
	#include "tft_drivers/hx8340b/8/setxy.h"
#endif
#ifndef DISABLE_HX8340B_S
	#include "tft_drivers/hx8340b/s/setxy.h"
#endif
#ifndef DISABLE_ST7735
	#include "tft_drivers/st7735/setxy.h"
#endif
#ifndef DISABLE_S1D19122
	#include "tft_drivers/s1d19122/setxy.h"
#endif
#ifndef DISABLE_PCF8833
	#include "tft_drivers/pcf8833/setxy.h"
#endif
#ifndef DISABLE_SSD1963_480
	#include "tft_drivers/ssd1963/480/setxy.h"
#endif
#ifndef DISABLE_SSD1963_800
	#include "tft_drivers/ssd1963/800/setxy.h"
#endif
#ifndef DISABLE_S6D1121
	#include "tft_drivers/s6d1121/setxy.h"
#endif
#ifndef DISABLE_ILI9320
	#include "tft_drivers/ili9320/setxy.h"
#endif
	}
}
void clrXY()
{
	if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}
void drawRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void drawRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void fillRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	if (display_transfer_mode==16)
	{
		cbi(PORT_CS, BIT_CS);
		setXY(x1, y1, x2, y2);
		sbi(PORT_RS, BIT_RS);
		_fast_fill_16(fch,fcl,((long(x2-x1)+1)*(long(y2-y1)+1)));
		sbi(PORT_CS, BIT_CS);
	}
	else if ((display_transfer_mode==8) and (fch==fcl))
	{
		cbi(PORT_CS, BIT_CS);
		setXY(x1, y1, x2, y2);
		sbi(PORT_RS, BIT_RS);
		_fast_fill_8(fch,((long(x2-x1)+1)*(long(y2-y1)+1)));
		sbi(PORT_CS, BIT_CS);
	}
	else
	{
		if (orient==PORTRAIT)
		{
			for (int i=0; i<((y2-y1)/2)+1; i++)
			{
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
		else
		{
			for (int i=0; i<((x2-x1)/2)+1; i++)
			{
				drawVLine(x1+i, y1, y2-y1);
				drawVLine(x2-i, y1, y2-y1);
			}
		}
	}
}

void fillRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
 
	cbi(PORT_CS, BIT_CS);
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(fch,fcl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(fch,fcl);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(fch,fcl);
	}
	sbi(PORT_CS, BIT_CS);
	clrXY();
}

void fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}

void clrScr()
{
	long i;
	
	cbi(PORT_CS, BIT_CS);
	clrXY();
	if (display_transfer_mode!=1)
		sbi(PORT_RS, BIT_RS);
	if (display_transfer_mode==16)
		_fast_fill_16(0,0,((disp_x_size+1)*(disp_y_size+1)));
	else if (display_transfer_mode==8)
		_fast_fill_8(0,((disp_x_size+1)*(disp_y_size+1)));
	else
	{
		for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
		{
			if (display_transfer_mode!=1)
				LCD_Writ_Bus(0,0,display_transfer_mode);
			else
			{
				LCD_Writ_Bus(1,0,display_transfer_mode);
				LCD_Writ_Bus(1,0,display_transfer_mode);
			}
		}
	}
	sbi(PORT_CS, BIT_CS);
}

void fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void fillScr(word color)
{
	long i;
	char ch, cl;
	
	ch=color>>8;
	cl=color & 0xFF;

	cbi(PORT_CS, BIT_CS);
	clrXY();
	if (display_transfer_mode!=1)
		sbi(PORT_RS, BIT_RS);
	if (display_transfer_mode==16)
		_fast_fill_16(ch,cl,((disp_x_size+1)*(disp_y_size+1)));
	else if ((display_transfer_mode==8) and (ch==cl))
		_fast_fill_8(ch,((disp_x_size+1)*(disp_y_size+1)));
	else
	{
		for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
		{
			if (display_transfer_mode!=1)
				LCD_Writ_Bus(ch,cl,display_transfer_mode);
			else
			{
				LCD_Writ_Bus(1,ch,display_transfer_mode);
				LCD_Writ_Bus(1,cl,display_transfer_mode);
			}
		}
	}
	sbi(PORT_CS, BIT_CS);
}

void setColor(byte r, byte g, byte b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void setColor(word color)
{
	fch=color>>8;
	fcl=color & 0xFF;
}

word getColor()
{
	return (fch<<8) | fcl;
}

void setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
}

void setBackColor(word color)
{
	bch=color>>8;
	bcl=color & 0xFF;
}

word getBackColor()
{
	return (bch<<8) | bcl;
}

void setPixel(word color)
{
	LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void drawPixel(int x, int y)
{
	cbi(PORT_CS, BIT_CS);
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	sbi(PORT_CS, BIT_CS);
	clrXY();
}

void drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		cbi(PORT_CS, BIT_CS);
		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
		sbi(PORT_CS, BIT_CS);
	}
	clrXY();
}

void drawHLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	cbi(PORT_CS, BIT_CS);
	setXY(x, y, x+l, y);
	if (display_transfer_mode == 16)
	{
		sbi(PORT_RS, BIT_RS);
		_fast_fill_16(fch,fcl,l);
	}
	else if ((display_transfer_mode==8) and (fch==fcl))
	{
		sbi(PORT_RS, BIT_RS);
		_fast_fill_8(fch,l);
	}
	else
	{
		for (int i=0; i<l+1; i++)
		{
			LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(PORT_CS, BIT_CS);
	clrXY();
}

void drawVLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	cbi(PORT_CS, BIT_CS);
	setXY(x, y, x, y+l);
	if (display_transfer_mode == 16)
	{
		sbi(PORT_RS, BIT_RS);
		_fast_fill_16(fch,fcl,l);
	}
	else if ((display_transfer_mode==8) and (fch==fcl))
	{
		sbi(PORT_RS, BIT_RS);
		_fast_fill_8(fch,l);
	}
	else
	{
		for (int i=0; i<l+1; i++)
		{
			LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(PORT_CS, BIT_CS);
	clrXY();
}

void printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 

	cbi(PORT_CS, BIT_CS);
  
	if (orient==PORTRAIT)
	{
		setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
		{
			ch=pgm_read_byte(&cfont.font[temp]);
			for(i=0;i<8;i++)
			{   
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else
				{
					setPixel((bch<<8)|bcl);
				}   
			}
			temp++;
		}
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

		for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
		{
			setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
			for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<i))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					}   
				}
			}
			temp+=(cfont.x_size/8);
		}
	}
	sbi(PORT_CS, BIT_CS);
	clrXY();
}

void rotateChar(byte c, int x, int y, int pos, int deg)
{
	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  

	cbi(PORT_CS, BIT_CS);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else  
				{
					setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	sbi(PORT_CS, BIT_CS);
	clrXY();
}

void print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

void printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
	int c2;
	long mult;
	unsigned long inum;
  
	if (dec<1)
	dec=1;
	if (dec>5)
	dec=5;
	  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-2-dec); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=divider;
			for (int i=0; i<dec; i++)
				st[c+2+i]=48;
			st[c+2+dec]=0;
		}
		else
		{
			st[0]=48;
			st[1]=divider;
			for (int i=0; i<dec; i++)
				st[2+i]=48;
			st[2+dec]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		mult = 1;
		for (int j = 0; j < dec; j++)
			mult = mult * 10;
		inum=long(num*mult + 0.5);
	  
		while (inum>0)
		{
			buf[c]=48+(inum % 10);
			c++;
			inum=(inum-(inum % 10))/10;
		}
		if ((num<1) and (num>0))
		{
			buf[c]=48;
			c++;
		}
		while (c<(dec+1))
		{
			buf[c]=48;
			c++;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg-1))
		{
			for (int i=0; i<(length-c-neg-1); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		c2=neg;
		for (int i=0; i<c; i++)
		{
			st[c2+f]=buf[c-i-1];
			c2++;
			if ((c-(c2-neg))==dec)
			{
				st[c2+f]=divider;
				c2++;
			}
		}
		st[c2+f]=0;
	}

	print(st,x,y);
}

void setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

uint8_t* getFont()
{
	return cfont.font;
}

uint8_t getFontXsize()
{
	return cfont.x_size;
}

uint8_t getFontYsize()
{
	return cfont.y_size;
}

void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;
	byte r, g, b;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			cbi(PORT_CS, BIT_CS);
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
			sbi(PORT_CS, BIT_CS);
		}
		else
		{
			cbi(PORT_CS, BIT_CS);
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx; tx>=0; tx--)
				{
					col=pgm_read_word(&data[(ty*sx)+tx]);
					LCD_Write_DATA(col>>8,col & 0xff);
				}
			}
			sbi(PORT_CS, BIT_CS);
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			cbi(PORT_CS, BIT_CS);
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
			}
			sbi(PORT_CS, BIT_CS);
		}
		else
		{
			cbi(PORT_CS, BIT_CS);
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx; tx>=0; tx--)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}
			sbi(PORT_CS, BIT_CS);
		}
	}
	clrXY();
}

void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	byte r, g, b;
	double radian;
	radian=deg*0.0175;  

	if (deg==0)
		drawBitmap(x, y, sx, sy, data);
	else
	{
		cbi(PORT_CS, BIT_CS);
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		sbi(PORT_CS, BIT_CS);
	}
	clrXY();
}

void lcdOff()
{
	cbi(PORT_CS, BIT_CS);
	switch (display_model)
	{
	case PCF8833:
		LCD_Write_COM(0x28);
		break;
	}
	sbi(PORT_CS, BIT_CS);
}

void lcdOn()
{
	cbi(PORT_CS, BIT_CS);
	switch (display_model)
	{
	case PCF8833:
		LCD_Write_COM(0x29);
		break;
	}
	sbi(PORT_CS, BIT_CS);
}

void setContrast(char c)
{
	cbi(PORT_CS, BIT_CS);
	switch (display_model)
	{
	case PCF8833:
		if (c>64) c=64;
		LCD_Write_COM(0x25);
		LCD_Write_DATA(c);
		break;
	}
	sbi(PORT_CS, BIT_CS);
}

int getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}
