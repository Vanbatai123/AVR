/*
 * KS0108.c
 *
 * Created: 8/11/2018 9:48:18 PM
 *  Author: ASUS PC
 */ 


#include "../include.h"
#include "KS0108.h"


void Ks0108_Wait()
{
	_delay_ms(1);
}

void Ks0108_WriteCommand(uint8_t cmd)
{
	while (Ks0108_ReadStatus() == BUSY_FLAG);
	
	setb(CTRL_PORT, EN);
	clrb(CTRL_PORT, RS);
	clrb(CTRL_PORT, RW);
	
	DATA_PORT = cmd;
	
	clrb(CTRL_PORT, EN);
}
void Ks0108_WriteData(uint8_t ddata)
{
	while (Ks0108_ReadStatus() == BUSY_FLAG);
	
	setb(CTRL_PORT, EN);
	setb(CTRL_PORT, RS);
	clrb(CTRL_PORT, RW);
	
	DATA_PORT = ddata;
	
	clrb(CTRL_PORT, EN);
}

void Ks0108_ChipSelect(uint8_t chip)
{
	switch (chip)
	{
		case 1:
			clrb(CTRL_PORT, CS1);
			setb(CTRL_PORT, CS2);
			setb(CTRL_PORT, CS3);
			break;
		case 2:
			setb(CTRL_PORT, CS1);
			clrb(CTRL_PORT, CS2);
			setb(CTRL_PORT, CS3);
			break;
		case 3:
			setb(CTRL_PORT, CS1);
			setb(CTRL_PORT, CS2);
			clrb(CTRL_PORT, CS3);
			break;
		default:
		break;
	}
}


uint8_t Ks0108_ReadStatus()
{
	Ks0108_Wait();
	clrb(CTRL_PORT, EN);
	
	setb(CTRL_PORT, RW);
	clrb(CTRL_PORT, RS);
	
	Ks0108_ChipSelect(1);
	DATA_DDR = 0x00;
	
	setb(CTRL_PORT, EN);
	Ks0108_Wait();
	uint8_t flag = DATA_PIN;
	DATA_DDR = 0xFF;
	
	clrb(CTRL_PORT, EN);
	
	return flag;
}

void Ks0108_Config()
{
	DATA_DDR = CTRL_DDR = 0xFF;
	clrb(CTRL_PORT, EN);
}