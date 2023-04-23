/* 				
/			PS2_LOW.H
/
/		Low level PS2 protocol driver
/
/		Author: Visakhan C
/
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define DDR_CLOCK DDRB
#define PORT_CLOCK PORTB
#define PIN_CLOCK PINB
#define CLOCK_PIN 2

#define DDR_DATA DDRB
#define PORT_DATA PORTB
#define PIN_DATA PINB
#define DATA_PIN 1

  



void PS2_Init(void);
void PS2_SendToDevice(uint8_t);
