/*
 * ST7628.c
 *
 * Created: 1/19/2019 12:12:21 PM
 * Author : Van_BasTai	
 */ 

#include "include.h"
#include "uart/uart.h"
//lcd 98x70
#define ST7628_PORT			PORTD
#define ST7628_DDR			DDRD

#define ST7628_PIN_CS		5
#define ST7628_PIN_RST		4
#define ST7628_PIN_CLK		3
#define ST7628_PIN_MOSI		2

#define TFTWIDTH  97
#define TFTHEIGHT 69

#define NOP     0x00
#define SWRESET 0x01
#define BSTRON	0x03
#define RDDID   0x04
#define RDDST   0x09

#define SLPIN   0x10
#define SLPOUT  0x11
#define PTLON   0x12
#define NORON   0x13

#define INVOFF  0x20
#define INVON   0x21
#define WRCNTR  0x25
#define DISPOFF 0x28
#define DISPON  0x29
#define CASET   0x2A
#define RASET   0x2B
#define RAMWR   0x2C
#define RAMRD   0x2E

#define PTLAR   0x30
#define COLMOD  0x3A
#define MADCTL  0x36
#define IDMOFF 0x38

#define FRMCTR1 0xB1
#define FRMCTR2 0xB2
#define FRMCTR3 0xB3
#define INVCTR  0xB4
#define NLIS       0xB5
#define DISSET5 0xB6

#define PWCTR1  0xC0
#define PWCTR2  0xC1
#define PWCTR3  0xC2
#define PWCTR4  0xC3
#define PWCTR5  0xC4
#define VMCTR1  0xC5
#define VSS         0xCB
#define ID1S        0xCC
#define ID3S        0xCE

#define ANASET 0xD0
#define ALS       0xD7
#define RDID1   0xDA
#define RDID2   0xDB
#define RDID3   0xDC
#define RDID4   0xDD

#define PWCTR6  0xFC
#define THYS       0xF7

#define GMCTRP1 0xE0
#define GMCTRN1 0xE1
#define EPMRD     0xE3

// Color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
void ST7628_writeCmd(uint8_t cmd)
{
	clrb(ST7628_PORT, ST7628_PIN_CS);
	clrb(ST7628_PORT, ST7628_PIN_MOSI);// enter write command
	
	setb(ST7628_PORT, ST7628_PIN_CLK);
	clrb(ST7628_PORT, ST7628_PIN_CLK);
	
	for (int8_t i = 7; i >= 0; i--)
	{
		if (inbit(cmd, i))
		{
			setb(ST7628_PORT, ST7628_PIN_MOSI);
		}
		else
		{
			clrb(ST7628_PORT, ST7628_PIN_MOSI);
		}
		
		setb(ST7628_PORT, ST7628_PIN_CLK);
		clrb(ST7628_PORT, ST7628_PIN_CLK);
	}
	setb(ST7628_PORT, ST7628_PIN_CS);
}

void ST7628_writeData(uint8_t data)
{
	clrb(ST7628_PORT, ST7628_PIN_CS);
	setb(ST7628_PORT, ST7628_PIN_MOSI);// enter write data
	
	setb(ST7628_PORT, ST7628_PIN_CLK);
	clrb(ST7628_PORT, ST7628_PIN_CLK);
	
	for (int8_t i = 7; i >= 0; i--)
	{
		if (inbit(data, i))
		{
			setb(ST7628_PORT, ST7628_PIN_MOSI);
		}
		else
		{
			clrb(ST7628_PORT, ST7628_PIN_MOSI);
		}
		
		setb(ST7628_PORT, ST7628_PIN_CLK);
		clrb(ST7628_PORT, ST7628_PIN_CLK);
	}
	setb(ST7628_PORT, ST7628_PIN_CS);
}


// fill a rectangle
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
uint16_t color) {

	// rudimentary clipping (drawChar w/big text requires this)
	//if((x + w - 1) >= _width)  w = _width  - x;
	//if((y + h - 1) >= _height) h = _height - y;

	//setAddrWindow(x, y, x+w, y+h);

	ST7628_writeCmd(0x2A);
	ST7628_writeData(x);
	ST7628_writeData(x+w);
	ST7628_writeCmd(0x2B);
	ST7628_writeData(y);
	ST7628_writeData(y+h);
	ST7628_writeCmd(0x2C);
	
	uint8_t hi = color >> 8, lo = color;

	
	for(y=h; y>=0; y--) {
		for(x=w; x>=0; x--) {
			ST7628_writeData(hi);
			ST7628_writeData(lo);
		}
	}

}
void fillScreen(uint16_t color) {
	fillRect(0, 0,  97, 69, color);
}



int main(void)
{
	UART_config(115200);
	UART_println("UART OK!!!");
	
    ST7628_PORT &= ~(1<<ST7628_PIN_CLK);
    ST7628_DDR |= (1<<ST7628_PIN_MOSI) | (1<<ST7628_PIN_CS) | (1<<ST7628_PIN_CLK);
	ST7628_DDR |= (1<<ST7628_PIN_RST);
	
	ST7628_PORT |= (1<<ST7628_PIN_RST);
	_delay_ms(500);
	ST7628_PORT &= ~(1<<ST7628_PIN_RST);
	_delay_ms(500);
	ST7628_PORT |= (1<<ST7628_PIN_RST);
	_delay_ms(500);
	
	// ST7628 config
	ST7628_writeCmd(WRCNTR);
	ST7628_writeData(0x39);
	
	ST7628_writeCmd(SLPOUT);
	ST7628_writeCmd(NORON);
	ST7628_writeCmd(INVOFF);
	ST7628_writeCmd(IDMOFF);
	
	ST7628_writeCmd(MADCTL);
	ST7628_writeData(0x00);
	ST7628_writeCmd(COLMOD);
	ST7628_writeData(0x05);
	ST7628_writeCmd(DISPON);
	UART_println("111!!!");
	UART_println("LCD OK!!!");
	while (1) 
    {
		
		_delay_ms(500);
		
		fillScreen(rand() * rand());
    }
}

