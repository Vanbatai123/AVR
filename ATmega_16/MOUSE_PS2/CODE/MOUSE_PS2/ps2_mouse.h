/* 				
/			PS2_MOUSE.H
/
/		 PS/2 Mouse driver
/
*/


#include "E:\AVR\MOUSE_PS2\CODE\MOUSE_PS2\ps2_low.c"

#define BUFF_SIZE 3

#define PS2_RESET 0xFF
#define PS2_REPORT_EN 0xF4
#define PS2_ACK 0xFA
#define PS2_ERROR 0xFC
#define PS2_BAT_SUCCESS 0xAA
#define PS2_MOUSE_ID 0x00

#define MAX_CUR_X 15
#define MIN_CUR_X 0
#define MAX_CUR_Y 1
#define MIN_CUR_Y 0

typedef enum { INIT,ID_OK,BAT_PASSED,ERROR,ACK} PS2_STATUS;
typedef enum { FALSE,TRUE } BOOL;

void MouseInit(void);
void Decode(uint8_t);
uint8_t MouseGetX(void);
uint8_t MouseGetY(void);
uint8_t MouseGetButtons(void);

