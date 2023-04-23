/*
 * TAOXUNG.c
 *
 * Created: 10/18/2016 6:40:39 PM
 *  Author: Administrator
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__// mo khoa ham delay ???????????????
#include <util/delay.h>
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))

int main(void)
{
	DDRA=0xff;
    while(1)
    {
        setb(PORTA,0);
		clrb(PORTA,1);
		_delay_ms(10);
		setb(PORTA,1);
		clrb(PORTA,0);
		_delay_ms(10);
    }
}