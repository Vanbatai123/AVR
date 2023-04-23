/*
 * doduong.c
 *
 * Created: 11/25/2015 3:03:06 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
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
	setb(ADMUX,REFS0);//chon dien ap tham chieu
	//setb(ADMUX,MUX0);
	setb(ADCSRA,ADPS2);//CAP XUNG NHIP
	setb(ADCSRA,ADEN);
	while(1)
	{
		int gtcb=0;
		move_LCD(1,1);
		int gtadc;
		for (int i=0;i<8;i++)
		{
			mabat(i);//chon chan adc
			setb(ADCSRA,ADSC);//BAT DAU CHUYEN DOI
			while(inbit(ADCSRA,ADIF)==0);//cho cho chuyen doi thanh cong
			gtadc=ADCW;//DOC GIA TRI LUU
			if (gtadc<700)
			{
				putChar_LCD('1');
				gtcb=(gtcb<<1)+1;
			}
			else
			{
				putChar_LCD('0');
				gtcb=gtcb<<1;
			}
			//putChar_LCD(' ');
			
		}
		putChar_LCD(' ');
		move_LCD(1,11);
		vietso(gtcb);
		_delay_ms(11);
	}
}