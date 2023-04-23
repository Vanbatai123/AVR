/*
* LCD_HP.c
*
* Created: 4/10/2018 8:50:59 PM
*  Author: Van_BasTai
*/


#define LCDHP_PORT			PORTA
#define LCDHP_DDR			DDRA

#define LCDHP_CS			4 // chip select
#define LCDHP_SDA			5 // data pin
#define LCDHP_IO			6 // latch data
#define LCDHP_SCK			7 // clock pin


/*
 * LCD write function
 */
void LCDHP_Write(uint8_t cd, uint8_t byte);
/*
 * Configuration LCD
 */
void LCDHP_Config();
/*
 * LCD move cursor
 */
void LCDHP_Move(uint8_t x, uint8_t y);
/*
 * LCD clear screen
 */
void LCDHP_Clear();
/*
 * LCD print character
 */
void LCDHP_PutC(uint8_t dat);
/*
 * LCD print string
 */
void LCDHP_PutS(char *s);
/*
 * Display a float number
 */
void LCDHP_PutF(double number, uint8_t digits);
/*
 * display number
 * digit: DEC, HEX, BIN
 */
void LCDHP_PutNum(double num, uint8_t digit);
/*
 * move 1 pixel
 */
void LCDHP_Move1Pixel(uint8_t x, uint8_t y);
/*
 * Display picture
 */
void picture(int dat);
/*
 * insert a space
 */
void LCDHP_Space(unsigned char num);