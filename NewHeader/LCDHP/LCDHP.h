/*
* LCD_HP.c
*
* Created: 4/10/2018 8:50:59 PM
*  Author: Van_BasTai
*/


#define LCDHP_PORT		PORTC
#define LCDHP_DDR		DDRC

#define LCDHP_PIN_CS	0 // Chip select pin
#define LCDHP_PIN_SDA	1 // Data pin
#define LCDHP_PIN_AO	2 // Command or data (C/D)
#define LCDHP_PIN_CLK	3 // Clock pin


void LCDHP_write(uint8_t cd, uint8_t byte);// LCD write command or data
void LCDHP_config();// configuration
void LCDHP_goTo(uint8_t x, uint8_t y);// move cursor to a point
void LCDHP_clear();// clear screen
void LCDHP_putC(uint8_t dat);// display a character to
void LCDHP_putC_R(uint8_t dat);// display a character reserve
void LCDHP_print(char *str);// display a string to LCD
void LCDHP_printNum(long num, uint8_t base);// display a number to LCD
void LCDHP_printFloat(double num, uint8_t digits);// display a float number
void LCDHP_goToPixel(uint8_t x, uint8_t y);// move cursor by 1 pixel
void LCDHP_picture(int dat);// display picture to LCD
void LCDHP_space(uint8_t num);// make a space character