#include <avr/io.h>
#define F_CPU 8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__// mo khoa ham delay ???????????????
#include <util/delay.h>
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#include <stdlib.h>
#define x_limit           800
#define y_limit           600
// Alphanumeric LCD Module functions
#define  lcd_port PORTC

//#include <lcd.h>
char i=0;
char mask[] =
{
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80
};

char oddParity(char byte)
{
	char i , count = 0
	for(i=0 ; i<8 ; i++)
	{
		if(mask[i] & byte) count++;
	}
	
	return ((count+1)%2) ;
}

char mouseStatus =  0;
//0 : no activity
//1 : falling edge handled
//2 : rising edge handled
//Introduces delay in case of parity error
char mouseData = 0; //received data will be stored here temp
char mouseIndex = 0; //progress
char mouseFlag=0; //1: data is received  2: Parity not matched

int mouseX = 0;
int mouseY = 0;
char isLeft = 0;
char isRight = 0;
char isMiddle = 0;

readMouse()
{
	
	if(mouseStatus==0 && DDRB.0==0 && PINB.0==0 && DDRB.1==0 && PINB.1==0) //if inactive and start bit is there
	{
		mouseStatus = 1; //start bit received
		mouseIndex = 0;  //initialise
		mouseData = 0;   //initialise
		mouseFlag = 0;   //initialise
	}
	
	//if falling edge handled
	//look for rising edge and handle it
	else if(mouseStatus==1 && DDRB.0==0 && PINB.0==1) mouseStatus = 2;
	
	//if rising edge handled
	//look for falling edge and handle it
	//data is scanned at falling edge
	else if(mouseStatus==2 && DDRB.0==0 && PINB.0==0)
	{
		if(mouseIndex<8 && PINB.1==1) mouseData += (mask[mouseIndex]);
		mouseIndex++;
		mouseStatus = 1;
	}
	
	// stop bit
	if(mouseIndex>9) //all eight bytes have been received AND stp bit gone
	{
		mouseStatus = 0;
		mouseIndex = 0;
		//processMouseData();
		mouseFlag=1;
	}
}


void releaseClockMouse()
{
	DDRB.0 = 0;
	PORTB.0 = 1;
}
void releaseDataMouse()
{
	DDRB.1 = 0;
	PORTB.1 = 1;
}
void holdClockMouse()
{
	PORTB.0 = 0;
	DDRB.0 = 1;
}
void holdDataMouse()
{
	PORTB.1 = 0;
	DDRB.1 = 1;
}
void writeMouse(char cmd)
{
	//Release both the lines
	releaseDataMouse();
	releaseClockMouse();
	
	//Bring Clock line low for at least 100 uS
	holdClockMouse(); delay_us(120);	         //may not be working now it may be
	
	//Bring the Data line low
	holdDataMouse();
	
	//Release the Clock line
	releaseClockMouse();
	
	// wait for the clock high
	while(PINB.0==0);
	
	for(i=0 ; i<=7 ; i++) //8 data bits
	{
		//Wait for device to bring the clock low
		while(PINB.0==1);
		//Set the data bit
		if(cmd & mask[i]) /*== 1)*/ releaseDataMouse();  //CCCHHHHAAANGED
		else holdDataMouse();
		//Wait for devise to bring the clock high
		while(PINB.0==0);
	}
	
	//Wait for devise to bring the clock low
	while(PINB.0==1);
	
	//Set the parity
	if(oddParity(cmd)==1)                        	//try reversing the parity
	{
		releaseDataMouse();
	}
	else
	{
		holdDataMouse();
	}
	//Wait for devise to bring the clock high
	while(PINB.0==0);
	
	//Wait for device to bring the clock low
	while(PINB.0==1);
	//Release the data line
	releaseDataMouse();
	//wait for clock high
	while(PINB.0==0);
	//Wait for device to bring data low
	while(PINB.1==1);
	//Wait for device to bring clock low
	while(PINB.0==1);
	//Wait for device to release clock and data
	while(PINB.0==0 || PINB.1==0);
}

mouseInit()
{
	while(1)
	{
		delay_ms(100); //just for the sake of delay
		//reset
		writeMouse(0xFF);
		//receive 0xFA - acknowledgment
		while(!mouseFlag) readMouse();
		if(mouseFlag==1 && mouseData==0xFA) mouseFlag=0;
		else continue;
		//receive 0xAA - self test passed
		while(!mouseFlag) readMouse();
		if(mouseFlag==1 && mouseData==0xAA) mouseFlag=0;
		else continue;
		//receive 0x00 - device ID
		while(!mouseFlag) readMouse();
		if(mouseFlag==1 && mouseData==0x00) mouseFlag=0;
		else continue;
		
		//Enter remote mode
		writeMouse(0xF0);
		//receive 0xFA - acknowledgment
		while(!mouseFlag) readMouse();
		if(mouseFlag==1 && mouseData==0xFA) mouseFlag=0;
		else continue;
		break;
	}
}

scanMouse()
{
	//stores data from mouse
	char byte1 = 0;
	char byte2 = 0;
	char byte3 = 0;
	char byte4 = 0;
	int i;
	writeMouse(0xEB); //get data from mouse
	
	while(!mouseFlag) readMouse();
	if(mouseFlag==1 && mouseData==0xFA) mouseFlag=0;
	else return;
	
	
	while(!mouseFlag) readMouse();
	if(mouseFlag==1)
	{
		byte1 = mouseData;
		mouseFlag=0;
	}
	else {
		if(mouseFlag==2)
		return;
	}
	while(!mouseFlag) readMouse();
	if(mouseFlag==1)
	{
		byte2 = mouseData;
		mouseFlag=0;
	}
	else  return;
	
	
	while(!mouseFlag) readMouse();
	if(mouseFlag==1)
	{
		byte3 = mouseData;
		mouseFlag=0;
	}
	else return;
	
	//Byte processing
	if(byte1 & 0x01) isLeft=1;
	else isLeft=0;
	
	if(byte1 & 0x02) isRight=1;
	else isRight=0;
	
	if(byte1 & 0x04) isMiddle=1;
	else isMiddle=0;
	
	if(!(byte1 & 0x10)) //+ve X movement
	{
		if(x_limit-byte2 > mouseX) mouseX += byte2;
		else mouseX = x_limit; //mouse reached end of the screen
	}
	else            //-ve X movement
	{
		if(255-byte2+1 < mouseX) mouseX -= (255-byte2+1);
		else mouseX = 0; //mouse reached begining of the screen
	}
	
	
	if(!(byte1 & 0x20)) //+ve Y movement
	{
		if(y_limit-byte3 > mouseY) mouseY += byte3;
		else mouseY = y_limit; //mouse reached end of the screen
	}
	else            //-ve Y movement
	{
		if(255-byte3+1 < mouseY) mouseY -= (255-byte3+1);
		else mouseY = 0; //mouse reached begining of the screen
	}
}

//Mouse Clockline == PINB.0;
//Mouse Dataline  == PINB.1;

void main(void)
{
	char c[20];
	
	// LCD module initialization
	lcd_init(16);
	
	DDRB.3 = 1;
	PORTB.3 = 1;
	
	releaseDataMouse();
	releaseClockMouse();
	
	delay_ms(500);
	lcd_clear();
	mouseInit();
	delay_ms(1000);
	lcd_clear();
	while (1)
	{
		lcd_clear();
		lcd_gotoxy(0,0);
		scanMouse();
		lcd_putsf("X:");
		itoa(mouseX,c);
		lcd_puts(c);
		lcd_gotoxy(0,1);
		lcd_putsf("Y:");
		itoa(mouseY,c);
		lcd_puts(c);
		lcd_gotoxy(8,1);
		if(isLeft)
		lcd_putsf(“left”);
		else if(isRight)
		lcd_putsf(“right”);
		else if(isMiddle)
		lcd_putsf(“middle”);
		else
		lcd_putsf(“no click”)
		delay_ms(50);
	};
}

