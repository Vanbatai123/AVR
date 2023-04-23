/*
* spi.h
*
* Created: 11/15/2018 9:11:45 PM
*  Author: Van_BasTai
*/


#ifndef SPI_H_
#define SPI_H_

#ifdef UDR // ATmega16
#define SPI_PIN_SCK		7
#define SPI_PIN_MISO	6
#define SPI_PIN_MOSI	5
#define SPI_PIN_SS		4
#endif

#ifdef UDR0 // ATmega328
#define SPI_PIN_SCK		5
#define SPI_PIN_MISO	4
#define SPI_PIN_MOSI	3
#define SPI_PIN_SS		2
#endif

#define SPI_DDR			DDRB
#define SPI_PORT		PORTB

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#define LSBFIRST			0
#define MSBFIRST			1

#define SPI_CLOCK_DIV4		0x00
#define SPI_CLOCK_DIV16		0x01
#define SPI_CLOCK_DIV64		0x02
#define SPI_CLOCK_DIV128	0x03
#define SPI_CLOCK_DIV2		0x04
#define SPI_CLOCK_DIV8		0x05
#define SPI_CLOCK_DIV32		0x06

#define SPI_MODE0			0x00
#define SPI_MODE1			0x04
#define SPI_MODE2			0x08
#define SPI_MODE3			0x0C

#define SPI_MODE_MASTER		0x00
#define SPI_MODE_SLAVE		0x01

#define SPI_MODE_MASK		0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK		0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK	0x01  // SPI2X = bit 0 on SPSR




// Initialize the SPI library
// argument: SPI_MODE_MASTER / SPI_MODE_SLAVE
void SPI_begin(uint8_t spi_mode);
uint8_t SPI_transfer(uint8_t data);
uint16_t SPI_transfer16(uint16_t data);
void SPI_transferBuf(void *buf, size_t count);
void SPI_end();
// LSBFIRST/ MSBFIRST
void SPI_setBitOrder(uint8_t bitOrder);
// SPI_MODE0~3
void SPI_setDataMode(uint8_t dataMode);
void SPI_beginTransaction(uint8_t bitOrder, uint8_t clockDevider, uint8_t mode);
void SPI_setClockDivider(uint8_t clockDiv);
void SPI_attachInterrupt();
void SPI_detachInterrupt();

#endif /* SPI_H_ */