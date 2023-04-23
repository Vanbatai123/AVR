/*
* LCD192x64.c
*
* Created: 1/28/2017 10:43:29 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "myUART.h"


#define KS0108_SCREEN_WIDTH		192
#define KS0108_SCREEN_WIDTH		192
#define KS0108_SCREEN_HEIGHT	64


#define DISPLAY_SET_Y       0x40
#define DISPLAY_SET_X       0xB8
#define DISPLAY_START_LINE  0xC0
#define DISPLAY_ON_CMD		0x3E
#define ON	0x01
#define OFF	0x00
#define DISPLAY_STATUS_BUSY	0x80

#define KS0108_DATA_PORT	PORTC
#define KS0108_DATA_DIR		DDRC
#define KS0108_DATA_PIN		PINC

#define KS0108_CTRL_PORT	PORTA
#define KS0108_CTRL_DIR		DDRA

#define KS0108_RS			(1 << 0)
#define KS0108_RW			(1 << 1)
#define KS0108_EN			(1 << 2)

#define KS0108_CS1			(1 << 3)
#define KS0108_CS2			(1 << 4)
#define KS0108_CS3			(1 << 5)

extern unsigned char screen_x;
extern unsigned char screen_y;


//-------------------------------------------------------------------------------------------------
// Delay function
//-------------------------------------------------------------------------------------------------
void GLCD_Delay(void)
{
	asm("nop");
}
//-------------------------------------------------------------------------------------------------
// Enalbe Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_EnableController(unsigned char controller)
{
	switch(controller){
		case 0 : KS0108_CTRL_PORT &= ~KS0108_CS1; break;
		case 1 : KS0108_CTRL_PORT &= ~KS0108_CS2; break;
		case 2 : KS0108_CTRL_PORT &= ~KS0108_CS3; break;
	}
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_DisableController(unsigned char controller)
{
	switch(controller){
		case 0 : KS0108_CTRL_PORT |= KS0108_CS1; break;
		case 1 : KS0108_CTRL_PORT |= KS0108_CS2; break;
		case 2 : KS0108_CTRL_PORT |= KS0108_CS3; break;
	}
}
//-------------------------------------------------------------------------------------------------
// Read Status from specified controller (0-2)
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadStatus(unsigned char controller)
{
	unsigned char status;
	KS0108_DATA_DIR = 0x00;
	KS0108_CTRL_PORT |= KS0108_RW;
	KS0108_CTRL_PORT &= ~KS0108_RS;
	GLCD_EnableController(controller);
	KS0108_CTRL_PORT |= KS0108_EN;
	GLCD_Delay();
	status = KS0108_DATA_PIN;
	KS0108_CTRL_PORT &= ~KS0108_EN;
	GLCD_DisableController(controller);
	return status;
}
//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char commandToWrite, unsigned char controller)
{
	while(GLCD_ReadStatus(controller)&DISPLAY_STATUS_BUSY);
	KS0108_DATA_DIR = 0xFF;
	KS0108_CTRL_PORT &= ~(KS0108_RW | KS0108_RS);
	GLCD_EnableController(controller);
	KS0108_DATA_PORT = commandToWrite;
	KS0108_CTRL_PORT |= KS0108_EN;
	GLCD_Delay();
	KS0108_CTRL_PORT &= ~KS0108_EN;
	GLCD_DisableController(controller);
}
//-------------------------------------------------------------------------------------------------
// Read data from current position
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadData(void)
{
	unsigned char data;
	while(GLCD_ReadStatus(screen_x / 64)&DISPLAY_STATUS_BUSY);
	KS0108_DATA_DIR = 0x00;
	KS0108_CTRL_PORT |= (KS0108_RW | KS0108_RS);
	GLCD_EnableController(screen_x / 64);
	//GLCD_Delay();
	KS0108_CTRL_PORT |= KS0108_EN;
	GLCD_Delay();
	data = KS0108_DATA_PIN;
	KS0108_CTRL_PORT &= ~KS0108_EN;
	GLCD_DisableController(screen_x / 64);
	screen_x++;
	return data;
}
//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char dataToWrite)
{
	while(GLCD_ReadStatus(screen_x / 64)&DISPLAY_STATUS_BUSY);
	KS0108_DATA_DIR = 0xFF;
	KS0108_CTRL_PORT &= ~KS0108_RW;
	KS0108_CTRL_PORT |= KS0108_RS;
	KS0108_DATA_PORT = dataToWrite;
	GLCD_EnableController(screen_x / 64);
	KS0108_CTRL_PORT |= KS0108_EN;
	GLCD_Delay();
	KS0108_CTRL_PORT &= ~KS0108_EN;
	GLCD_DisableController(screen_x / 64);
	screen_x++;
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_InitalizePorts(void)
{
	KS0108_CTRL_DIR =0xff;
	KS0108_CTRL_PORT =0xff;
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadByteFromROMMemory(char * ptr)
{
	return pgm_read_byte(ptr);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
unsigned char screen_x = 0, screen_y = 0;
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_Initalize(void)
{
	unsigned char i;
	GLCD_InitalizePorts();
	for(i = 0; i < 3; i++)
	GLCD_WriteCommand((DISPLAY_ON_CMD | ON), i);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_GoTo(unsigned char x, unsigned char y)
{
	unsigned char i;
	screen_x = x;
	screen_y = y;

	for(i = 0; i < KS0108_SCREEN_WIDTH/64; i++)
	{
		vietso_dec_UART(i);
		GLCD_WriteCommand(DISPLAY_SET_Y | 0,i);
		print_UART("G.1\r\n");
		GLCD_WriteCommand(DISPLAY_SET_X | y,i);
		print_UART("G.2\r\n");
		GLCD_WriteCommand(DISPLAY_START_LINE | 0,i);
		print_UART("G.3\r\n");
	}
	GLCD_WriteCommand(DISPLAY_SET_Y | (x % 64), (x / 64));
	GLCD_WriteCommand(DISPLAY_SET_X | y, (x / 64));
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearScreen(void)
{
	print_UART("1.1\r\n");
	unsigned char i, j;
	for(j = 0; j < KS0108_SCREEN_HEIGHT/8; j++)
	{
		GLCD_GoTo(0,j);
		for(i = 0; i < KS0108_SCREEN_WIDTH; i++) GLCD_WriteData(0x00);
		print_UART("1.2\r\n");
	}
	print_UART("1.3\r\n");
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(char charToWrite)
{
	int i;
	charToWrite -= 32;
	for(i = 0; i < 5; i++)
	//GLCD_WriteData(GLCD_ReadByteFromROMMemory((char *)((int)font5x8 + (5 * charToWrite) + i)));
	GLCD_WriteData(0x00);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(char * stringToWrite)
{
	while(*stringToWrite)
	GLCD_WriteChar(*stringToWrite++);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
	unsigned char tmp;
	GLCD_GoTo(x, (y / 8));
	tmp = GLCD_ReadData();
	GLCD_GoTo(x, (y / 8));
	tmp = GLCD_ReadData();
	GLCD_GoTo(x, (y / 8));
	tmp |= (1 << (y % 8));
	GLCD_WriteData(tmp);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(char * bmp, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy)
{
	unsigned char i, j;
	for(j = 0; j < dy / 8; j++)
	{
		GLCD_GoTo(x,y + j);
		for(i = 0; i < dx; i++)
		GLCD_WriteData(GLCD_ReadByteFromROMMemory(bmp++));
	}
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

int main(void)
{
	//init_UART();
	//print_UART("OKKKKK\r\n");
	//GLCD_Initalize();
	//print_UART("1\r\n");
	//GLCD_ClearScreen();
	//print_UART("2\r\n");
	DDRD = 0xff;
	//GLCD_GoTo(1, 1);
	while(1)
	{
		
	}
}