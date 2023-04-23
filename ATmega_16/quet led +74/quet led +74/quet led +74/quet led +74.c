/*
 * _74HC595.c
 *
 * Created: 10/22/2015 11:07:52 AM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
char ma[]={0X3F,0x06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
///#define data_in clrb(PORTC,0)
//#define clk clrb(PORTC,1)
//#define latch clrb(PORTC,2)
#include <avr/io.h>
void truyen595(int data)
{
	for (int i=8;i>0;i--)
	{
		
		if((data&0x80)==0x80)
		{setb(PORTA,0);}
		else
		{clrb(PORTA,0);}
		data=data<<1;
		//xung clk cua thanh ghi dich
		clrb(PORTA,1);
		setb(PORTA,1);
	}
	//xung chot
	clrb(PORTA,2);
	setb(PORTA,2);
	
	
}
void quetled(int so)
{
	int n,t,c,dv;
	
	n=so/1000;
	t=(so%1000)/100;
	c=(so%100)/10;
	dv=so%10;
	
	
	PORTA=0X00;
	PORTD=0xFE;
	truyen595(ma[dv]);
	_delay_us(900);
	
	PORTA=0X00;
	PORTD=0xFD;
	truyen595(ma[c]);
	_delay_us(900);
	
	
	PORTA=0X00;
	PORTD=0xFB;
	truyen595(ma[t]);
	_delay_us(900);
	
	PORTA=0X00;
	PORTD=0xF7;
	truyen595(ma[n]);
	_delay_us(900);
}

int main(void)
{
	DDRD=0xff;
	PORTA=0X00;
	DDRA=0XFF;
	PORTD=0X00;
	int a=2345;
    while(1)
    {
		
		
			
			quetled(a);
			
		
		a++;
		if (a==10000)
		{
			a=0;
		}
       
    }
}