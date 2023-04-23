/*
 * DOTRO.c
 *
 * Created: 11/20/2015 4:36:29 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) thanhghi=(thanhghi^(1<<bit))
#include "E:\AVR\Header\myLCD_HP.h"
int so[]={'0','1','2','3','4','5','6','7','8','9'};
void mabat(int bin)
{
	if (bin==0)
	{
		clrb(ADMUX,MUX0);
		clrb(ADMUX,MUX1);
		clrb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==1)
	{
		setb(ADMUX,MUX0);
		clrb(ADMUX,MUX1);
		clrb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==2)
	{
		clrb(ADMUX,MUX0);
		setb(ADMUX,MUX1);
		clrb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==3)
	{
		setb(ADMUX,MUX0);
		setb(ADMUX,MUX1);
		clrb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==4)
	{
		clrb(ADMUX,MUX0);
		clrb(ADMUX,MUX1);
		setb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==5)
	{
		setb(ADMUX,MUX0);
		clrb(ADMUX,MUX1);
		setb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==6)
	{
		clrb(ADMUX,MUX0);
		setb(ADMUX,MUX1);
		setb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
	else if (bin==7)
	{
		setb(ADMUX,MUX0);
		setb(ADMUX,MUX1);
		setb(ADMUX,MUX2);
		clrb(ADMUX,MUX3);
		clrb(ADMUX,MUX4);
	}
}
int main(void)
{
	DDRC=0xff;
	DDRD=0xff;
	init_LCD();
    clr_LCD();
	setb(ADMUX,REFS0);
    //setb(ADMUX,MUX0);
    setb(ADCSRA,ADPS2);
    setb(ADCSRA,ADEN);
	while(1)
    {
		//home_LCD();
		long gtadc;
		float r;
		mabat(0);
		setb(ADCSRA,ADSC);
		while(inbit(ADCSRA,ADIF)==0);
		gtadc=ADCW;
		r=385*gtadc/(1023-gtadc);
		move_LCD(1,1);
		print_LCD("X=");
		vietso_LCD(gtadc);
		_delay_ms(10);
		mabat(1);
		setb(ADCSRA,ADSC);
		while(inbit(ADCSRA,ADIF)==0);
		gtadc=ADCW;
		r=385*gtadc/(1023-gtadc);
		
		move_LCD(2,1);
		print_LCD("Y=");
		vietso_LCD(gtadc);
		_delay_ms(200);
		move_LCD(1,1);
		print_LCD("       ");
		move_LCD(2,1);
		print_LCD("       ");
		
    }
}