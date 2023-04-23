/*
 * LCD1602.h
 *
 * Created: 3/23/2015 9:28:08 AM
 *  Author: Van_BasTai
 */ 

#define LCD1602_PIN_EN		3
#define LCD1602_PIN_RS		2

#define LCD1602_DDR			DDRD
#define LCD1602_PORT		PORTD
#define LCD1602_PIN			PIND


// private function
char Read2Nib();
void Write2Nib(uint8_t chr);
void wait_LCD();

/*
 * Configuration LCD
 */
void LCD1602_Config();
/*
 * Return cursor to home
 */
void LCD1602_Home();
/*
 * Move cursor position
 */
void LCD1602_Move(uint8_t y, uint8_t x);
/*
 * Display a character
 */
void LCD1602_PutC(uint8_t chr);
/*
 * Display a string
 */
void LCD1602_PutS(char* str);
/*
 * Display a number
 */
void LCD1602_PutNum(double num, uint8_t digit);

/*
 * Display a float number
 */
void LCD1602_PutF(double num, uint8_t digits);
/*
 * Clear screen
 */
void LCD1602_Clear();