/*
 * lcd.c
 *
 * Created: 10/27/2015 8:11:05 AM
 *  Author: Administrator
 */ 
#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myUART.h"
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
	init_UART();
    setb(ADMUX,REFS0);
    //setb(ADMUX,MUX0);
    setb(ADCSRA,ADPS2);
    setb(ADCSRA,ADEN);
	while(1)
    {
		float gtadc;
		mabat(2);
		setb(ADCSRA,ADSC);
		while(inbit(ADCSRA,ADIF)==0);
		gtadc=ADCW;
		//if (gtadc<700)
		vietso_UART(gtadc);
		print_UART("\t");
		vietso_UART(gtadc*5000/1023);
		_delay_ms(100);
		print_UART("\r");
    }
}