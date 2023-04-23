/*
 * i2c.h
 *
 * Created: 11/2/2018 9:03:31 PM
 *  Author: Van_BasTai
 */ 


#ifndef I2C_H_
#define I2C_H_


#define I2C_TIMEOUT	2000

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif
uint8_t rxBufferIndex;
uint8_t rxBufferLength;
uint8_t rxBuffer[32];

uint8_t txBufferIndex;
uint8_t txBufferLength;
uint8_t txBuffer[32];


void I2C_begin(uint8_t addr);
uint8_t I2C_beginTransmission(uint8_t addr);
void I2C_write(uint8_t data);
uint8_t I2C_endTransmission();
void I2C_writeBuffer(uint8_t *data, uint8_t len);
void I2C_requestFrom(uint8_t addr, uint8_t len);
uint8_t I2C_read();
int16_t I2C_available();
void I2C_onReceive(void(*function)(void));
void I2C_onRequest(void(*function)(void));
void I2C_slaveWrite(uint8_t datasend[]);
#endif