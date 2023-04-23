/*
 * KS0108.h
 *
 * Created: 8/11/2018 9:48:34 PM
 *  Author: ASUS PC
 */ 


#ifndef KS0108_H_
#define KS0108_H_


#define DATA_DDR		DDRA
#define DATA_PORT		PORTA
#define DATA_PIN		PINA

#define CTRL_DDR		DDRC
#define CTRL_PORT		PORTC

// Control pin
#define RS				0
#define RW				1
#define EN				2

#define CS1				3
#define CS2				4
#define CS3				5

// Instruction
#define DISPLAY_ON		0x3F
#define DISPLAY_OFF		0x3E

#define SET_Y_ADDR		0x40
#define SET_X_ADDR		0xB8
#define START_LINE		0xC0

// Status read
#define BUSY_FLAG		0x80
#define DISPLAY_FLAG	0x20


void Ks0108_Wait();
void Ks0108_ChipSelect(uint8_t chip);
uint8_t Ks0108_ReadStatus();
void Ks0108_WriteCommand(uint8_t cmd);
void Ks0108_WriteData(uint8_t ddata);
void Ks0108_Config();



#endif /* KS0108_H_ */