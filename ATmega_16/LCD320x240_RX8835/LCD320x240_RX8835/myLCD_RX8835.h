/*
* myLCD_RX8835.h
*
* Created: 8/2/2018 11:29:39 PM
*  Author: ASUS PC
*/


#ifndef MYLCD_RX8835_H_
#define MYLCD_RX8835_H_


#define RD			0
#define WR			1
#define A0			2

#define DATA_DDR	DDRA
#define DATA_PORT	PORTA
#define DATA_PIN	PINA

#define CTRL_DDR	DDRC
#define CTRL_PORT	PORTC
#define CTRL_PIN	PINC

/************************************************************************/
/*							COMMAND SET									*/
/************************************************************************/

//SYSTEM CONTROL
#define SYSTEM_SET	0x40 //Initialize device and display
#define SLEEP_IN	0x53 //Enter standby mode


//DISPLAY CONTROL
#define DISPLAY_ON	0x59 //Enable  display and display flashing
#define DISPLAY_OFF	0x58 //Disable display and display flashing
#define SCROLL		0x44 //Set display start address and display regions
#define CSRFORM		0x5D //Set cursor type
#define CGRAM_ADR	0x5C //Set start address of character generator RAM
#define CSRDIR		0x4C // - 0x4F //Set direction of cursor movement
#define HDOT_SCR	0x5A //Set horizontal scroll position
#define OVLAY		0x5B //Set display overlay format

//DRAWING CONTROL
#define CSRW		0x46 //Set cursor address
#define CSRR		0x47 //Read cursor address

//MEMORY CONTROL
#define MWRITE		0x42 //Write to display memory
#define MREAD		0x43 //Read from display memory

/************************************************************************/
/* Rx8835 Basiccal Function												*/
/************************************************************************/

void Rx8835_Wait();
void Rx8835_WriteCommand(uint8_t);
void Rx8835_WriteData(uint8_t);
void Rx8835_SetCursorAddress(uint16_t addr);
void Rx8835_FillGraphic(uint16_t parameter);
void Rx8835_FillText(char cara);
void Rx8835_WriteCommand(uint8_t cmd);
void Rx8835_WriteData(uint8_t dt);
void Rx8835_Config();
void Rx8835_PutChar_x8(uint8_t chr);
void Rx8835_Print_x8(char* str);
void Rx8835_GoTo(uint16_t y, uint16_t x);
uint8_t Rx8835_ReadData();
uint8_t Rx8835_ReadBusyFlag();


/************************************************************************/
/* Rx8835 Graphic Function                                              */
/************************************************************************/

uint8_t Rx8835_PutChar_x16(uint8_t y, uint8_t x, uint8_t chr);
void Rx8835_Print_x16(uint8_t y, uint8_t x, char *str);
void Rx8835_PutChar_x29(uint8_t y, uint8_t x, uint8_t numb);
void Rx8835_PutChar_x53(uint8_t y, uint8_t x, uint8_t numb);
void Rx8835_print_Num_KPa(uint8_t y, uint8_t x, uint16_t val);
void Rx8835_print_Num_s(uint8_t y, uint8_t x, uint16_t val);
void Rx8835_DrawForm();

void Rx8835_PrintData(uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4, uint16_t val5, uint16_t val6);



#endif /* MYLCD_RX8835_H_ */