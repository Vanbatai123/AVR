/***********************************************************
/
/	Mouse driver program
/
/	May 25, 2012
/	Update: June 6, 2012: Mouse cursor change is now handled by driver itself, so only a call to the driver to
/					update the cursor is required.
/					Response to all 3 mouse buttons is shown on the LCD
/
***********************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "ps2_mouse.h"

#define LEFT_BUTTON 0x01   //mouse button state bits
#define RIGHT_BUTTON 0x02
#define MIDDLE_BUTTON 0x04
#define LCD_X_MAX 1

static void avr_init(void);
static void UpdateCursor(void);

static uint8_t lcd_x,lcd_y; 

uint8_t mouse_cursor[] PROGMEM = 
{
	0b00000000,
	0b00000000,
	0b00000000,
	0b00001110,
	0b00001100,
	0b00001010,
	0b00000001,
	0b00000000
};

//MAIN
int main(void)
{
	uint8_t button_state;
	avr_init();
    
	lcdClear();
	
	
    for(;;)
    {
       			
		UpdateCursor();
		_delay_ms(30);  //screen refresh rate
		
		//display cursor position
		lcdHome();
		lcdNumberSmall(lcd_x);
		lcdChar(',');
		lcdNumberSmall(lcd_y);
		
		//if you click a mouse button, display a symbol
		button_state = MouseGetButtons();
		if(button_state) {
		
			lcdGotoXY(lcd_x,lcd_y);
			switch( button_state ) {
	 
				case LEFT_BUTTON	: lcdChar('L'); break;
				
				case RIGHT_BUTTON	: lcdChar('R'); break;
				
				case MIDDLE_BUTTON	: lcdChar('M'); break;
				
			}
			_delay_ms(100);
		}
		
    }
	
    return(0);
}

static void avr_init(void)
{
	lcdInit();
	lcdDefineChar(mouse_cursor,0);
	MouseInit();
	
	return;
}

static void UpdateCursor(void)
{
	//erase cursor
	lcdGotoXY(lcd_x,lcd_y);
	lcdChar(' ');
	
	//Get current cordinates
	lcd_x = 1-MouseGetY();  //lcd x position changes with mouse y-movement
	lcd_y = MouseGetX();  //and vice-versa

	//display cursor
	lcdGotoXY(lcd_x,lcd_y);
	lcdChar(0);
}