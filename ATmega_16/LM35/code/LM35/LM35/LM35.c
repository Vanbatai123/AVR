/*
 * LM35.c
 *
 * Created: 12/31/2015 3:28:47 PM
 *  Author: Administrator
 */ 



#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#include "E:\AVR\Header\myLCD_HP.h"
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
	//clr_LCD();
	setb(ADMUX,REFS0);
	//setb(ADMUX,MUX0);
	setb(ADCSRA,ADPS2);
	setb(ADCSRA,ADEN); //cho phep hoat dong
	while(1)
	{
		//int gtcb=0;
		int temp;
		move_LCD(1,1);
		int gtadc;
		mabat(2);
		setb(ADCSRA,ADSC);
		while(inbit(ADCSRA,ADIF)==0);
		gtadc=ADCW;
		temp=(float)(gtadc*4.88/10);
		//temp=temp%1000;
		print_LCD("ADC=");
		vietso_LCD(gtadc);
		move_LCD(2,1);
		print_LCD("  T= ");
		vietso_LCD(temp);
		print_LCD("    ");
		_delay_ms(500);
		/*for (int i=0;i<8;i++)
		{
			mabat(i);
			setb(ADCSRA,ADSC);
			while(inbit(ADCSRA,ADIF)==0);
			gtadc=ADCW;
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
		_delay_ms(11);*/
	}
}