/*
* i2c.c
*
* Created: 11/2/2018 9:03:19 PM
*  Author: Van_BasTai
*/

#include "../include.h"
#include <util/twi.h>
#include "i2c.h"



static volatile voidFuncPtr onRequestFunc = NULL;
static volatile voidFuncPtr onReceiveFunc = NULL;

void I2C_Begin(uint8_t addr)
{
	rxBufferIndex = 0;
	rxBufferLength = 0;
	
	clrb(TWSR, TWPS0);
	clrb(TWSR, TWPS1);
	
	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

	// set addr
	TWAR = addr << 1;
	// enable twi module, acks, and twi interrupt
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
}

void I2C_BeginTransmission(uint8_t addr)
{
	txBufferIndex = 0;
	txBufferLength = 0;
	// send start condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	while(inbit(TWCR, TWINT) == 0); // Wait till start condition is transmitted
	while(TW_STATUS != TW_START);// wait for start condition
	// send 7bit addr
	TWDR = (addr << 1) + TW_WRITE;    // addr and write instruction (DATA = addr + WRITE)
	TWCR = (1 << TWINT) | (1 << TWEN);    // Clear TWI interrupt flag, Enable TWI
	while (inbit(TWCR, TWINT) == 0); // Wait till complete TWDR byte transmitted
	while(TW_STATUS != TW_MT_SLA_ACK);  // Check for the acknoledgement
}

void I2C_Write(uint8_t data)
{
	txBuffer[txBufferIndex++] = data;
	txBufferLength = txBufferIndex;
}

void I2C_EndTransmission()
{
	for(uint8_t i = 0; i < txBufferLength; ++i)
	{
		// write 1 byte data
		TWDR = txBuffer[i];    // put data in TWDR
		TWCR = (1 << TWINT) | (1 << TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (inbit(TWCR, TWINT) == 0); // Wait till complete TWDR byte transmitted
		while(TW_STATUS != TW_MT_DATA_ACK);  // Check for the acknoledgement
	}
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	while(!inbit(TWCR, TWSTO));  // Wait till stop condition is transmitted
}

void I2C_WriteBuffer(uint8_t *data, uint8_t len)
{
	for(uint8_t i = 0; i < len; ++i)
	{
		I2C_Write(data[i]);
	}
}

void I2C_RequestFrom(uint8_t addr, uint8_t len)
{
	_delay_ms(1);
	uint8_t i;
	rxBufferLength = len;
	rxBufferIndex = 0;

	// send start condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	while(inbit(TWCR, TWINT) == 0); // Wait till start condition is transmitted
	while(TW_STATUS != TW_START);// wait for start condition

	// send 7bit addr
	TWDR = (addr << 1) + TW_READ;    // addr and write instruction (DATA = addr + WRITE)
	TWCR = (1 << TWINT) | (1 << TWEN);    // Clear TWI interrupt flag, Enable TWI
	while(inbit(TWCR, TWINT) == 0); // Wait till complete TWDR byte transmitted
	while(TW_STATUS != TW_MR_SLA_ACK);  // Check for the acknoledgement
	
	// receive len - 1 byte
	for (i = 0; i < rxBufferLength - 1; i++)
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);					 //xoa TWINT,se goi ACK sau khi nhan moi byte
		while (inbit(TWCR, TWINT) == 0); // Wait till complete TWDR byte transmitted
		while(TW_STATUS != TW_MR_DATA_ACK);  // Check for the acknoledgement
		rxBuffer[i] = TWDR;			//`
	}

	// receive the last byte
	TWCR = (1 << TWINT) | (1 <<TWEN); 	//xoa TWINT de nhan byte cuoi, sau do set NOT ACK
	while(inbit(TWCR, TWINT) == 0); // Wait till complete TWDR byte transmitted
	while(TW_STATUS != TW_MR_DATA_NACK);  // Check for the acknoledgement
	rxBuffer[rxBufferLength - 1] = TWDR;

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	while(!inbit(TWCR, TWSTO));  // Wait till stop condition is transmitted
}

uint8_t I2C_Read()
{
	uint8_t value = -1;
	// get each successive byte on each call
	if(rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}
	return value;
}
// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int16_t I2C_available()
{
	return rxBufferLength - rxBufferIndex;
}

// sets function called on slave write
void I2C_onReceive( void (*function)(void) )
{
	onReceiveFunc = function;
}

// sets function called on slave read
void I2C_onRequest( void (*function)(void) )
{
	onRequestFunc = function;
}

void I2C_SlaveWrite(uint8_t datasend[])
{
	for (txBufferIndex = 0; txBufferIndex < 32; txBufferIndex++)
	{
		txBuffer[txBufferIndex] = datasend[txBufferIndex];
	}
	txBufferIndex = 0;
}

//---------------------------------------------SLAVE INTERRUPT------------------------------
ISR(TWI_vect){
	//UART_printNumln(TW_STATUS, HEX);
	switch (TW_STATUS){
		//**********************SLAVE RECEIVER MODE******************
	case TW_SR_SLA_ACK:		
		rxBufferIndex = 0;
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;	
	case TW_SR_DATA_ACK:
		rxBuffer[rxBufferIndex++] = TWDR;
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;
	case TW_SR_DATA_NACK:
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;	
	case TW_SR_GCALL_ACK:
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;		
	case TW_SR_GCALL_DATA_ACK:
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;
	case TW_SR_GCALL_DATA_NACK:
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;	
	case  TW_SR_STOP:
		rxBufferLength = rxBufferIndex;
		rxBufferIndex = 0;
		onReceiveFunc();
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK 
		break;
	//**********************END SLAVE RECEIVER MODE**************
	
	
	//--------------------SLAVE TRANSMITTER MODE----------------
	case TW_ST_SLA_ACK: 	//Match, Read is required
		onRequestFunc();
		txBufferIndex = 0;
		TWDR = txBuffer[txBufferIndex++];
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK
		break;
	case TW_ST_DATA_ACK:  	//data have been sent, get ACK then now send more
		TWDR = txBuffer[txBufferIndex++];
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK
		break;
	case TW_ST_ARB_LOST_SLA_ACK: 	 	//Lost Master but Match Read is required, so send data 
		TWDR;
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK	
		break;
	case TW_ST_DATA_NACK: 	//data have been sent, get NOT ACK then now stop, get back Slave mode	
		txBufferIndex = 0;	
		TWCR |= (1<<TWINT)|(1<<TWEA); //clear TWINT, set ACK
		break;
	//--------------------END SLAVE TRANSMITTER MODE------------	
	default:
		break;
	}
}

//------------------------------------------------------------------------------------------