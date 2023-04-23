#include <avr/io.h>
//#include "spi.h"

void spi_init(void)
{
    SPCR = 0x50;
    SPSR = 0x01;
}

unsigned char spi_tx(unsigned char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {
    }
    return SPDR;
}

unsigned char spi_rx(void)
{
    unsigned char tmp;
    SPDR = 0xff;
    while (!(SPSR & (1 << SPIF))) {
    }
    tmp = SPDR;
    return tmp;
}
