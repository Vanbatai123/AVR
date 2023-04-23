#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
int n=0;
int ma[]={
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
	,0xE3,0xC1,0x88,0x9C,0x9C,0x80,0x9C,0x9C//A
	,0xff,0xff
	,0xC0,0x80,0x8C,0xC0,0xC0,0x8C,0x80,0xC0//B
	,0xff,0xff
	,0xC1,0x80,0xF8,0xFC,0xFC,0xF8,0x80,0xC1//C
	,0xff,0xff
	,0xC0,0x80,0x8C,0x9C,0x9C,0x8C,0x80,0xC0//D
	,0xff,0xff
	,0x80,0x80,0xFC,0xE0,0xE0,0xFC,0x80,0x80//E
	,0xff,0xff
	,0xC0,0xC0,0xFC,0xE0,0xE0,0xFC,0xFC,0xFC//F
	,0xff,0xff
	,0xC1,0x80,0xF8,0xFC,0x8C,0x98,0x80,0xC1//G
	,0xff,0xff
	,0x9C,0x9C,0x9C,0x80,0x80,0x9C,0x9C,0x9C//H
	,0xff,0xff
	,0x81,0x81,0xE7,0xE7,0xE7,0xE7,0x81,0x81//I
	,0xff,0xff
	,0x83,0x83,0xCF,0xCF,0xCF,0xCB,0xC3,0xE7//J
	,0xff,0xff
	,0x9C,0xCC,0xE4,0xF0,0xF0,0xE4,0xCC,0x9C //K
	,0xff,0xff
	,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0x81,0x81 //L
	,0xff,0xff
	,0x3C,0x18,0x00,0x24,0x3C,0x3C,0x3C,0x3C //M
	,0xff,0xff
	,0x9E,0x9C,0x98,0x90,0x84,0x8C,0x9C,0x9C //N
	,0xff,0xff
	,0xC1,0x80,0x9C,0x9C,0x9C,0x9C,0x80,0xC1 //O
	,0xff,0xff
	,0xE0,0xC0,0xCC,0xC0,0xE0,0xFC,0xFC,0xFC //P
	,0xff,0xff
	,0xC1,0x80,0x9C,0x9C,0x94,0x8C,0xC0,0xA1 //Q
	,0xff,0xff
	,0xE0,0xC0,0xCC,0xC0,0xE0,0xF0,0xE4,0xCC //R
	,0xff,0xff
	,0xE3,0xC1,0xD9,0xF3,0xE7,0xCD,0xC1,0xE3 //S
	,0xff,0xff
	,0x81,0x81,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7 //T
	,0xff,0xff
	,0x9C,0x9C,0x9C,0x9C,0x9C,0x88,0x80,0xC1//U
	,0xff,0xff
	,0x9C,0x9C,0x9C,0x9C,0x9C,0x88,0xC1,0xE3 //V
	,0xff,0xff
	,0x3C,0x3C,0x3C,0x3C,0x24,0x24,0x81,0xDB //W
	,0xff,0xff
	,0x7C,0x39,0x93,0xC7,0xC7,0x93,0x39,0x7C //X
	,0xff,0xff
	,0x7E,0x3C,0x99,0xC3,0xE7,0xE7,0xE7,0xE7 //Y
	,0xff,0xff
	,0x80,0x80,0xCF,0xE7,0xF3,0xF9,0x80,0x80 //Z
	,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
	

	

	
};
void quet()
{
	
	int x=n+8;
	for (int g=0;g<10;g++)
	{
		int j=0xFE;
		for (int i=n;i<x;i++)
		{
			PORTC=j;
			PORTA=~ma[i];
			_delay_ms(1);
			j<<=1;
			j++;
		}
	}
	n++;
	if (n==267)
	n=0;
}
int main(void)
{
	//int a=0;
	DDRA=0XFF;
	DDRC=0XFF;
	PORTA=0x00;
	while(1)
	{
		
		
		
		quet();
		
		
	}
}