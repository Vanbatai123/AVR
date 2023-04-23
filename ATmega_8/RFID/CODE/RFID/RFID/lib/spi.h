/*
 * spi.c
 * 
 * Copyright 2013 Shimon <shimon@monistit.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdint.h>

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SPI_MOSI	3
#define SPI_MISO	4
#define SPI_SS		2
#define SPI_SCK		5

#define ENABLE_CHIP() clrb(SPI_PORT, SPI_SS);
#define DISABLE_CHIP() setb(SPI_PORT, SPI_SS);



void spi_init()
{
	SPI_DDR |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);//prescaler 16
}

uint8_t spi_transmit(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

