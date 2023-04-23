/*
 * REMOTE_LC50J2 v2.c
 *
 * Created: 3/11/2016 10:15:59 PM
 *  Author: Van BasTai
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define DATA_DDR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PIND
#define DATA 2

#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) thanhghi=(thanhghi^(1<<bit))

#include "E:\AVR\Header\myUART.h"
#include "E:\AVR\Header\myLCD.h"

//int ma[]=	{	7674,7661,7657,7664,7656,7672,7652,7668,7660,7676,7650,7666,7658,7648,7659,7662,7675,7673,7677,7649,7653,7663,7679,7671,7655,7678,7667,7651};
//int kenh[]={'A','M','O','1','2','3','4','5','6','7','8','9','W','0','C','M','D','U','D','R','L','O','S','B','P','W','Q','Z'};
int ma[]={23461,31621,16321,24481,8161,28561,12241,20401,4081,30601,6121,14281,32641,27541,5101,1021,21421,17341,4846,11221,19381,11986,8926,16066,14026,9946,15046,10966,13006,28306,30346,26266,27286};
int kenh[]={'O','M','1','2','3','4','5','6','7','8','F','9','0','S','U','D','R','L','K','E','M','P','I','L','T','R','E','T','A','R','G','Y','B'};
int y=0;
int main(void)
{
	
	DDRA=0xff;
	DDRC=0xff;
	DDRB=0xff;
	DDRD=0x00;
	PORTD=0xff;
	PORTB=0x00;
	
	init_UART();
	//NGAT NGOAI
	setb(PORTD,0);
	MCUCR=(1<<ISC01)|(1<<ISC11);
	GICR=(1<<INT0)|(1<<INT1);
	sei();
	init_LCD();
	home_LCD();
	print_LCD("vanbatai");
	while(1)
	{
		
	}
}
ISR(INT0_vect)
{
	int i=0;
	char x=0;
	// xén phan mã remote giong nhau
	while(inbit(DATA_PIN,DATA)==0);       //?????????????????
	while(inbit(DATA_PIN,DATA)==1);
	while(1)
	{
		
		if(inbit(DATA_PIN,DATA)==0)
		{
			while(inbit(DATA_PIN,DATA) ==0);
			i++;
		}
		if(i==17)
			break;
	}
	// lay phan mã khác nhau 16 bit
	while(x<16)
	{		
		if(inbit(DATA_PIN,DATA)==1)
		{
			_delay_us(600);				// cho 700 us
			if(inbit(DATA_PIN,DATA)==1) // nu sau 700us mà tín hieu còn thì nó là bit 1 (dinh bang)
			{
				y=y*2+1;
				while(inbit(DATA_PIN,DATA)==1);
			}
			else						// nguoc lai là bit 0 (dinh nhon)
			{
				y=y*2;
			}
			
			x++;
		}
		else
			while(inbit(DATA_PIN,DATA)==0);	
	}
	y=0-y;	
	/*vietso_UART(y);
	putChar_UART(',');*/
	
	for (int h=0;h<28;h++)
	{
		if (y==ma[h])
		{
			//move_LCD(1,1);
			putChar_UART(kenh[h]);
			putChar_UART('\r');
			//print_LCD("           ");
		}
	}
	_delay_ms(100); // delay 100ms cho ket thúc phan tín hieu còn lai neu không khi thoát hàm ngat  nó se xay ra ngat tiep
}