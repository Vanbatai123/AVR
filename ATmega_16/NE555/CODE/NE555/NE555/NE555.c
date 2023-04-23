/*
 * NE555.c
 *
 * Created: 3/16/2016 11:25:03 AM
 *  Author: Administrator
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) thanhghi=(thanhghi^(1<<bit))

int main(void)
{
	DDRA=0XFF;
	DDRB=0xff;
    while(1)
    {
		daobit(PORTA,0);
		_delay_us(35);
			
        
    }
}