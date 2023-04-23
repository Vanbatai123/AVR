/*
 * spi.c
 *
 * Created: 11/15/2018 9:11:26 PM
 *  Author: PC
 */ 

#include "../include.h"
#include "SPI.h"



void begin()
{
	SPI_DDR |= (1 << SPI_PIN_SCK) | (1 << SPI_PIN_MOSI);
	SPI_PORT |= (1 << SPI_PIN_MISO); //dien tro keo len cho chan MISO
	SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << SPR1) | (1 << SPR0);
	//SPIE: interrupt, SPE: enable, MSTR: Master mode, CPHA: Phase, SPR1:0: pre-scaler = 128
}

void end() 
{
	
}

// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
void setBitOrder(uint8_t bitOrder)
{
	if (bitOrder == LSBFIRST) SPCR |= (1 <<DORD);
	else SPCR &= ~((1 <<DORD));
}
// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
void setDataMode(uint8_t dataMode)
{
	SPCR = (SPCR & ~SPI_MODE_MASK) | dataMode;
}
// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
void setClockDivider(uint8_t clockDiv)
{
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clockDiv & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clockDiv >> 2) & SPI_2XCLOCK_MASK);
}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t transfer(uint8_t data) {
	SPDR = data;
	/*
	* The following NOP introduces a small delay that can prevent the wait
	* loop form iterating when running at the maximum speed. This gives
	* about 10% more speed, even if it seems counter-intuitive. At lower
	* speeds it is unnoticed.
	*/
	asm volatile("nop");
	while (!(SPSR & (1 <<SPIF))) ; // wait
	return SPDR;
}
uint16_t transfer16(uint16_t data) {
	union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
	in.val = data;
	if (!(SPCR & (1 <<DORD))) {
		SPDR = in.msb;
		asm volatile("nop"); // See transfer(uint8_t) function
		while (!(SPSR & (1 <<SPIF))) ;
		out.msb = SPDR;
		SPDR = in.lsb;
		asm volatile("nop");
		while (!(SPSR & (1 <<SPIF))) ;
		out.lsb = SPDR;
		} else {
		SPDR = in.lsb;
		asm volatile("nop");
		while (!(SPSR & (1 <<SPIF))) ;
		out.lsb = SPDR;
		SPDR = in.msb;
		asm volatile("nop");
		while (!(SPSR & (1 <<SPIF))) ;
		out.msb = SPDR;
	}
	return out.val;
}
void transfer_buf(void *buf, size_t count) {
	if (count == 0) return;
	uint8_t *p = (uint8_t *)buf;
	SPDR = *p;
	while (--count > 0) {
		uint8_t out = *(p + 1);
		while (!(SPSR & (1 <<SPIF))) ;
		uint8_t in = SPDR;
		SPDR = out;
		*p++ = in;
	}
	while (!(SPSR & (1 <<SPIF))) ;
	*p = SPDR;
}

void sattachInterrupt() { SPCR |= (1 <<SPIE); }
void sdetachInterrupt() { SPCR &= ~(1 <<SPIE); }