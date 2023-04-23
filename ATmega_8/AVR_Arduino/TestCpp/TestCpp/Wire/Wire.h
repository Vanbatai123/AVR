/*
* I2C.h
*
* Created: 7/25/2018 3:51:03 PM
*  Author: taivb
*/


#ifndef I2C_H_
#define I2C_H_



class TwoWire
{
	public:
	TwoWire();
	void begin();
	void begin(int);
	void end();
	void setClock(uint32_t);
	
	void beginTransmission(int);
	uint8_t endTransmission(void);
	
	
	uint8_t requestFrom(int, int);
	size_t write(uint8_t);
	size_t write(const uint8_t *, size_t);
	
	int available(void);
	int read(void);
	int peek(void);
	void flush(void);
	void onReceive( void (*)(int) );
	void onRequest( void (*)(void) );

	
	uint8_t TWI_DS1307_wadr(uint8_t Addr);
	uint8_t TWI_DS1307_rblock(uint8_t Data[], uint8_t len );
	
	private:
	void begin(uint8_t);
	void beginTransmission(uint8_t);
	uint8_t endTransmission(uint8_t);
	uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
	uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
	uint8_t requestFrom(int, int, int);
	uint8_t requestFrom(uint8_t, uint8_t);
	
	static uint8_t rxBuffer[];
	static uint8_t rxBufferIndex;
	static uint8_t rxBufferLength;

	static uint8_t txAddress;
	static uint8_t txBuffer[];
	static uint8_t txBufferIndex;
	static uint8_t txBufferLength;

	static uint8_t transmitting;
	static void (*user_onRequest)(void);
	static void (*user_onReceive)(int);
	static void onRequestService(void);
	static void onReceiveService(uint8_t*, int);
};


#endif /* I2C_H_ */