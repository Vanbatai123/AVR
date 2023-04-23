/*
 * OSCILLOSCOPE.c
 *
 * Created: 12/29/2016 10:45:19 AM
 *  Author: Van_BasTai
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myTIMER_0.h"
#include "F:\AVR\Header\myADC.h"

int demm=0;
void oscilloscope(int bit)
{
	for (int t=1;t<5;t++)
	{
		if ((bit/8+1)==t)
		{
			for (int y=1;y<5;y++)
			{
				move_1pixel(y,demm);
				if(y==5-t)
					lcd_write(1,0x80>>(bit%8));
				else
					lcd_write(1,0);
			}
		}
	}
}
int main(void)
{
	_delay_ms(500);
	int div=1024;
	hesochia(div);
	init_ADC();
	init_LCD();
	move_1pixel(4,1);
	setb(TIMSK,TOIE0);
	sei();
	
    while(1)
    {
		
    }
}
ISR(TIMER0_OVF_vect)
{
	if (demm==128)
	{
		demm=0;
		move_1pixel(4,1);
	}
	demm++;
	//oscilloscope(read_ADC(1)*(1023/32)/1023);//chinh vi tri
	//oscilloscope(read_ADC(1)*(read_ADC(0)/32)/1023);
	oscilloscope(read_ADC(1)*25/1020);
	TCNT0=read_ADC(0)/4;//chinh do rong
	//TCNT0=123;
}