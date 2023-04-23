/*
* I2C.cpp
*
* Created: 7/25/2018 4:55:42 PM
*  Author: taivb
*/

#include "../include.h"
#include "Wire.h"


//FOR SLAVE USE*****************************
#define GENERAL_CALL	1	//1: Enable, 0: Disable General call
//code for SLAVE Receive----
#define OWN_ADDRESS_MATCH		0x60 //the own address is match,+W
#define OWN_LOST_MATCH			0x68 //arbitrary lost but still match
#define OWN_DATA_Rec_ACK		0x80 //own data,received then return ACK already
#define OWN_DATA_Rec_NACK		0x88 //own data,received then return NACK
#define STOP_Rec				0xA0

#define GENERAL_MATCH			0x70 //general call is match,
#define GENERAL_DATA_Rec_ACK	0x90 //general data,received then return ACK already
#define GENERAL_DATA_Rec_NACK	0x98 //general data,received then return NACK

//code for SLAVE Transmit----
#define ADDRESS_R_MATCH			0xA8 //the own address is match, Read is required
#define LOST_R_MATCH			0xB0 //Master is lost
#define DATA_Sent_ACK			0xB8 //data is sent, and ACK is received
#define DATA_Sent_NACK			0xC0 //data is sent, and NOT ACK is received, STOP will be sent soon
#define DATA_Last_Sent_ACK		0xC8 //last data is sent, and NOT ACK is received, STOP will be sent soon

//--rate at 8MHz of CPU_CLock
#define _222K	10
#define _100K	32

//--command for TWCR--------------
#define TWI_W	0
#define TWI_R	1

#define TWI_START	(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)	//0xA4
#define TWI_STOP	(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)	//0x94

#define TWI_Clear_TWINT	(1<<TWINT)|(1<<TWEN)			//0x84
#define TWI_Read_ACK	(1<<TWINT)|(1<<TWEA)|(1<<TWEN)	//0xC4 :clear TWINT for reading, set ACK after reading

//---Status for TWI Master--------
#define _START_Sent		0x08 //START Condition has been sent successful
#define	_R_START_Sent	0x10 //REPEAT START Condition has been sent successful

#define _SLA_W_ACK		0x18 //SLA address, Write bit has been sent, ACK has been received
#define _SLA_W_NACK		0x20 //SLA address, Write bit has been sent, NOT ACK has been received
#define _DATA_ACK		0x28 //DATA has been sent, ACK has been received
#define _DATA_NACK		0x30 //DATA has been sent, NOT ACK has been received

//--Master Receive-----------
#define _SLA_R_ACK		0x40 //SLA address, READ bit has been sent, ACK has been received
#define _SLA_R_NACK		0x48 //SLA address, READ bit has been sent, NOT ACK has been rece
#define _DATA_Rec_ACK	0x50 //DATA has been Received, ACK has been received
#define _DATA_Rec_NACK	0x58 //DATA has been Received, NOT ACK has been received


#define BUFFER_LENGTH 32
#define SDA 18
#define SCL	19

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);


// constructor
TwoWire::TwoWire()
{
}

void TwoWire::begin(void)
{
	//pinMode(SDA, OUTPUT);
	//pinMode(SCL, OUTPUT);
	
	TWSR = 0x00; //Prescaler=1
	TWBR = _100K;
	//TWAR = (Device_Addr << 1) + GENERAL_CALL;
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE); //ready for Slave mode: Ack is set, TWI is ENABLE and interrupt is ENABLE
	
}

static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

void twi_attachSlaveTxEvent( void (*function)(void) )
{
	twi_onSlaveTransmit = function;
}

void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
	twi_onSlaveReceive = function;
}

void TwoWire::begin(uint8_t address)
{
	
	//twi_attachSlaveTxEvent(onRequestService);
	//twi_attachSlaveRxEvent(onReceiveService);
	
	pinMode(SDA, OUTPUT);
	pinMode(SCL, OUTPUT);
	
	TWSR = 0x00; //Prescaler=1
	TWBR = _100K;
	TWAR = (address << 1);
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE); //ready for Slave mode: Ack is set, TWI is ENABLE and interrupt is ENABLE
}

void TwoWire::begin(int address)
{
	begin((uint8_t)address);
}

void TwoWire::end(void)
{
	//twi_disable();
	
	// disable twi module, acks, and twi interrupt
	TWCR &= ~((1 << TWEN) | (1 << TWIE) | (1 << TWEA));

	// deactivate internal pullups for twi.
	digitalWrite(SDA, 0);
	digitalWrite(SCL, 0);
}

void TwoWire::setClock(uint32_t clock)
{
	TWBR = ((F_CPU / clock) - 16) / 2;
}

void TwoWire::beginTransmission(uint8_t address)
{
	txAddress = address;
	txBufferIndex = 0;
	txBufferLength = 0;
	transmitting = 1;
	
	//Send START Condition
	TWCR=TWI_START;
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR & 0xF8) !=_START_Sent)
	{
		return;
	}; //START send error, escape anyway
	
	//Send Device Address---------------------
	TWDR=(txAddress<<1)+TWI_W; //repare address on TWDR,+W (send procedure)
	TWCR=TWI_Clear_TWINT; //start send address by cleaning TWINT
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR&0xF8) !=_SLA_W_ACK)
	{
		return;
	}; //addresssend error, escape anyway
	
}

void TwoWire::beginTransmission(int address)
{
	beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
	txBufferIndex = 0;
	txBufferLength = 0;
	transmitting = 0;
	
	TWCR = TWI_STOP;
	return 0;
}

uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
	uint8_t i;
	rxBufferLength = quantity;
	rxBufferIndex = 0;
	
	TWCR=TWI_START; // Start--------------------------------------------------------------------
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x08));	 //cho TWINT bit=1 va goi START thanh cong
	
	TWDR=(address<<1)+TWI_R; 							 //goi dia chi SLA +READ
	TWCR=TWI_Clear_TWINT; 									 //bat dau, xoa TWINT
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x40));	 //cho TWINT bit=1	va goi SLA thanh cong
	
	//nhan len-1 bytes dau tien---------------------
	for (i=0; i <quantity-1; i++)
	{
		TWCR=TWI_Read_ACK; 									 //xoa TWINT,se goi ACK sau khi nhan moi byte
		while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x50));//cho TWINT bit=1 hoac nhan duoc ACK
		rxBuffer[i]=TWDR;										  //doc du lieu vao mang Data
	}
	//nhan byte cuoi
	TWCR=TWI_Clear_TWINT; 									 //xoa TWINT de nhan byte cuoi, sau do set NOT ACK
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x58));   //cho TWIN=1 hoac trang thai not ack
	rxBuffer[quantity-1]=TWDR;
	
	TWCR=TWI_STOP;											 //STOP condition
	return 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

int TwoWire::read(void)
{
	int value = -1;
	
	// get each successive byte on each call
	if(rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;
}

size_t TwoWire::write(uint8_t val)
{
	//send DATA byte---------------------
	TWDR = val; //repare Data on TWDR
	TWCR = TWI_Clear_TWINT; //start send address by cleaning TWINT
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR & 0xF8) !=_DATA_ACK)
	{
		return (TWSR&0xF8);
	} //data send error, escape anyway
	
	return 0;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	if(transmitting)
	{
		// in master transmitter mode
		for(size_t i = 0; i < quantity; ++i)
		{
			write(data[i]);
		}
	}
	else
	{
		beginTransmission(txAddress);
		for(size_t i = 0; i < quantity; ++i)
		{
			write(data[i]);
		}
	}
	return quantity;
}

