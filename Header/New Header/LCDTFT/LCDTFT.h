/*
* LCDTFT.h
*
* Created: 12/13/2018 8:50:59 PM
*  Author: Van_BasTai
*/


#define swap(a,b) {int16_t t=a;a=b;b=t;}

#define DDR_DATA	DDRB
#define PORT_DATA	PORTB
#define PIN_DATA	PINB

#define DDR_CMD		DDRC
#define PORT_CMD	PORTC

#define SCREEN_COLOR BLACK
//-------------------------------
//Rotation Set
#define TOP			0
#define BOT			2
#define LEFT		1
#define RIGHT		3

#define LCD_RESET	0		//RESET
#define LCD_CS		1		//Chip Select
#define LCD_CD		2		//CMD, DATA
#define LCD_WR		3		//WRITE
#define LCD_RD		4		//READ

//Font Size
#define BIG			1
#define SMALL		0

#define RESET_IDLE			PORT_CMD |= (1<<LCD_RESET)
#define CS_IDLE				PORT_CMD |= (1<<LCD_CS)
#define WR_IDLE				PORT_CMD |= (1<<LCD_WR)
#define RD_IDLE				PORT_CMD |= (1<<LCD_RD)

#define RESET_ACTIVE		PORT_CMD &= ~(1<<LCD_RESET)
#define CS_ACTIVE			PORT_CMD &= ~(1<<LCD_CS)
#define WR_ACTIVE			PORT_CMD &= ~(1<<LCD_WR)
#define RD_ACTIVE			PORT_CMD &= ~(1<<LCD_RD)
#define CD_COMMAND			PORT_CMD &= ~(1<<LCD_CD)
#define CD_DATA				PORT_CMD |= (1<<LCD_CD)

#define BLACK				0x0000
#define BLUE				0x001F
#define RED					0xF800
#define GREEN				0x07E0
#define CYAN				0x07FF
#define MAGENTA				0xF81F
#define YELLOW				0xFFE0
#define WHITE				0xFFFF

#define	setReadDir()		DDR_DATA = 0x00
#define	setWriteDir()		DDR_DATA = 0xFF

#define WR_STROBE			{WR_ACTIVE;WR_IDLE;}



//-----------------------------------------------------------------------------------------------------------
void Port_init();
//-----------------------------------------------------------------------------------------------------------
void TFT_writeCommand(unsigned char cmd);
//-----------------------------------------------------------------------------------------------------------
void TFT_writeData(unsigned char dt);
//-----------------------------------------------------------------------------------------------------------
void TFT_write8(unsigned char dt);
//-----------------------------------------------------------------------------------------------------------
unsigned long TFT_readReg(unsigned char r);
//-----------------------------------------------------------------------------------------------------------
void TFT_setRotation(unsigned char r);
//-----------------------------------------------------------------------------------------------------------
void TFT_flood(unsigned short color,unsigned long len);
//-----------------------------------------------------------------------------------------------------------
void TFT_reset(void);
//-----------------------------------------------------------------------------------------------------------
void TFT_writeReg32(unsigned char r,unsigned long d);
//-----------------------------------------------------------------------------------------------------------
void TFT_setAddrWindow(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
//-----------------------------------------------------------------------------------------------------------
void TFT_drawPixel(int x,int y,unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_fillScreen(unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_fillRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_drawLineY(unsigned int x,unsigned int y1,unsigned int y2,unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_drawLineX(unsigned int x1,unsigned int x2,unsigned int y,unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_drawCircle(unsigned int x,unsigned int y,unsigned int radius,unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_drawRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
//-----------------------------------------------------------------------------------------------------------
unsigned int TFT_randColor();//random
//-----------------------------------------------------------------------------------------------------------
void TFT_putChar(unsigned int x, unsigned int y,unsigned char charr,unsigned char size, unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_print(unsigned int x, unsigned int y,char *text,unsigned char size, unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_Number_Hex(unsigned int x, unsigned int y,long number,unsigned char size, unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_Number_Dec(unsigned int x, unsigned int y,long number,unsigned char size, unsigned int color);
//-----------------------------------------------------------------------------------------------------------
void TFT_config(void);
//-----------------------------------------------------------------------------------------------------------