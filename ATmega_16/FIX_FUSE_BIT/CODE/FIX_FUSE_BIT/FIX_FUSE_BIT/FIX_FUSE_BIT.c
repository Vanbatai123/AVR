/*
 * FIX_FUSE_BIT.c
 *
 * Created: 11/11/2016 10:46:02 AM
 *  Author: Van_BasTai
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
int main(void)
{
    DDRA=0xff;
	while(1)
    {
		PORTA=0;PORTA=0;PORTA=0;PORTA=0;PORTA=0;PORTA=0;PORTA=0;
		PORTA=0xff;PORTA=0xff;PORTA=0xff;PORTA=0xff;PORTA=0xff;PORTA=0xff;PORTA=0xff;
    }

}