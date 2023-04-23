/*
* Z1.c
*
* Created: 7/23/2018 9:28:08 AM
*  Author: TaiVB
*/

#include "include.h"


#define RS 0
#define RW 1
#define EN 2

#define DATA_DDR	DDRA
#define DATA_PORT	PORTA
#define DATA_PIN	PINA

#define CTRL_DDR	DDRC
#define CTRL_PORT	PORTC
#define CTRL_PIN	PINC


//////////////////////////////////////////////////////////////////////////
// Command
#define CMD_CURSOR_HOME	0x02


#define BUSY_FLAG	0x80

#define MODE_GRAPHIC	0x36
#define MODE_NORMAL		0x30

#define BLINK_ON	(1 << 0)
#define CURSOR_ON	(1 << 1)
#define DISPLAY_ON	(1 << 2)

uint8_t mode;

void wait_LCD()
{
	_delay_ms(1);
}

uint8_t readBusyFlag()
{
	uint8_t flag;

	setb(CTRL_PORT, RW);
	clrb(CTRL_PORT, RS);
	
	setb(CTRL_PORT, EN); //enable
	DATA_PORT = 0xFF;
	DATA_DDR = 0x00; // data pin input
	
	_delay_ms(1);
	flag = DATA_PIN;
	
	clrb(CTRL_PORT, EN); //disable
	DATA_DDR = 0xFF;
	return flag;
}

uint8_t readData()
{
	uint8_t dt;

	setb(CTRL_PORT, RW);
	setb(CTRL_PORT, RS);
	
	DATA_PORT = 0xFF;
	DATA_DDR = 0x00; // data pin input
	
	setb(CTRL_PORT, EN); //enable
	
	_delay_ms(1);
	dt = DATA_PIN;
	
	clrb(CTRL_PORT, EN); //disable
	
	DATA_DDR = 0xFF;
	return dt;
}

void writeCmd(uint8_t cmd)
{
	while(readBusyFlag() & BUSY_FLAG);
	clrb(CTRL_PORT, RS);
	clrb(CTRL_PORT, RW);
	
	DATA_PORT = cmd;
	
	setb(CTRL_PORT, EN);
	clrb(CTRL_PORT, EN);
}

void writeData(uint8_t dt)
{
	while(readBusyFlag() & BUSY_FLAG);

	clrb(CTRL_PORT, RW);
	setb(CTRL_PORT, RS);
	
	DATA_PORT = dt;
	
	setb(CTRL_PORT, EN);
	clrb(CTRL_PORT, EN);
}

void init_LCD()
{
	
	_delay_ms(50);// delay for lcd ready
	
	DATA_DDR = 0xFF;
	CTRL_DDR |= _BV(EN) | _BV(RS) | _BV(RW);
	
	clrb(CTRL_PORT, EN);
	clrb(CTRL_PORT, RS);
	
	//Function set------------------------------------------------------------------------------
	writeCmd(MODE_NORMAL);//8 bit parallel interface
	
	//Display control---------------------------------------------------------------------------
	//writeCmd(0x08 | CURSOR_ON | BLINK_ON | DISPLAY_ON);// blink, cursor, display ON
	writeCmd(0x08 | DISPLAY_ON);// blink, cursor, display OFF
	
	//Entry mode set----------------------------------------------------------------------------
	writeCmd(0x06); //display left to right & not shift character
	
	mode = MODE_NORMAL;
	//writeCmd(0x36);//Graphich
}

void home_LCD()
{
	if (mode == MODE_GRAPHIC)
	{
		writeCmd(MODE_NORMAL);//8 bit parallel interface + normal
	}
	writeCmd(CMD_CURSOR_HOME);
	mode = MODE_NORMAL;
}

// y: line, x: column
void move_LCD(uint8_t y,uint8_t x)
{
	if (mode == MODE_GRAPHIC)
	{
		writeCmd(MODE_NORMAL);//8 bit parallel interface + normal
	}
	
	x--;
	y--;
	if (y < 0 || y > 4)	return;
	
	switch (y)
	{
		case 0:
		x |= 0x80 + x;
		break;
		case 1:
		x |= 0x90 + x;
		break;
		case 2:
		x |= 0x88 + x;
		break;
		case 3:
		x |= 0x98 + x;
		break;
		default:
		/* Your code here */
		break;
	}
	writeCmd(x);
	mode = MODE_NORMAL;
}

void putChar_LCD(uint8_t chr)
{
	if (mode == MODE_GRAPHIC)
	{
		writeCmd(MODE_NORMAL);//8 bit parallel interface + normal
	}
	writeData(chr);
	mode = MODE_NORMAL;
}

void print_LCD(char* str)
{
	uint8_t cnt = 0;
	while(str[cnt] != '\0')
	{
		putChar_LCD(str[cnt++]);
	}
}

void vietso_LCD(uint16_t num)
{
	char dis[10];
	sprintf(dis, "%d", num);
	print_LCD(dis);
}

void clr_LCD()
{
	if (mode == MODE_GRAPHIC)
	{
		writeCmd(MODE_NORMAL);//8 bit parallel interface + normal
	}
	writeCmd(0x01);
}

void clr_GLCD()
{
	if (mode == MODE_NORMAL)
	{
		writeCmd(MODE_GRAPHIC);//8 bit parallel interface + normal
	}
	
	for(uint8_t x = 0;x < 64; x++)
	{
		writeCmd(0x80 + x%32);// 32 line of first cell
		writeCmd(0x80 + x/32 * 8);// select cell 1 or 2
		for(uint8_t y = 0;y < 16; y++)	writeData(0x00);
	}
	mode = MODE_GRAPHIC;
}