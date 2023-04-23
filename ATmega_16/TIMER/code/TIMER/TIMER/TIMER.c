#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define trig 0
#define echo 1
int main(void)
{
	DDRC=0xff;
	DDRD=0xff;
	DDRA=0x01;
	init_LCD();
	clr_LCD();
	while(1)
	{
		long n;
		float d;
		
		/*/int x=1024;
		move_LCD(1,1);
		setb(PORTA,trig);
		_delay_ms(20);
		clrb(PORTA,trig);
		while(inbit(PINA,echo)==0);
		setb(TCCR0,CS00);
		//setb(TCCR0,CS01);
		//setb(TCCR0,CS02);
		TCNT0=0;
		while(inbit(PINA,echo)==1);
		n=TCNT0;
		d=(float)343*1/16000*n;
		vietso(d);
		_delay_ms(200);
		//move_LCD(2,1);*/
		move_LCD(1,1);
		print_LCD("DISTANCE=",9);
		move_LCD(1,10);
		setb(PORTA,trig);
		_delay_us(20);
		clrb(PORTA,trig);
		while(inbit(PINA,echo)==0);
		//setb(TCCR1B,CS10);
		setb(TCCR1B,CS11);
		//setb(TCCR1B,CS12);
		TCNT1=0;
		while(inbit(PINA,echo)==1);
		n=TCNT1;
		d=(float)(343*n/2000);
		vietso(d);
		_delay_ms(700);
	}
}