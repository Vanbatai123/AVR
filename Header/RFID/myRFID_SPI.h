/************************************************************************/
/*	FileName: myRFID_SPI.h												*/
/*	Created: 10/4/2017 7:20:57 PM										*/
/*  Author: Van_BasTai													*/
/* 																		*/
/*	Description: File Header giao tiep SPI								*/
/*																		*/
/************************************************************************/


#include <stdint.h>

//	Dinh nghia PORT IO
y
#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SPI_MOSI	3
#define SPI_MISO	4
#define SPI_SS		2
#define SPI_SCK		5

#define ENABLE_CHIP() clrb(SPI_PORT, SPI_SS);
#define DISABLE_CHIP() setb(SPI_PORT, SPI_SS);


//	Khoi tao SPI
void SPI_init()
{
	SPI_DDR |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);//prescaler 16
}
//	Truyen nhan du lieu SPI
uint8_t spi_transmit(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

