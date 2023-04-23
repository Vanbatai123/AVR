/*
 * do_an.c
 *
 * Created: 4/17/2017 11:08:11 AM
 * Author : tuanv
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "myLCD.h"
# define F_CPU 12000000UL
#define setb(thanhghi,bit) thanhghi=(1<<bit)|thanhghi
#define clb(thanhghi,bit) thanhghi=~((1<<bit))&thanhghi
#define inbit(thanhghi,bit) (thanhghi &(1<<bit))>>bit
#include <util/delay.h>
unsigned char adc,t;
 int8_t  mode;
int    a = 35;
// LCD 1602 che do 4 bit
// Chan du lieu
/* 
    D4 - PORTC.4
	D5 - PORTC.5
	D6 - PORTC.6
	D7 - PORTC.7
*/
// Chan dieu khien
/*
    RS - PORTC.2
	EN  - PORTC.3
*/

void read_adc(unsigned char kenh)
{
	ADMUX=(1<<REFS0)|kenh;
	setb(ADCSRA,ADSC);
	while(inbit(ADCSRA,ADIF)==0);
	adc=ADCW;
}
void temp()
{   
	read_adc(0);
	t=adc/2.048;
	LCD_Vitri(1,12);
	Truyendulieu((t/100)+48);
	LCD_Vitri(1,13);
	Truyendulieu((t%100/10)+48);
	LCD_Vitri(1,14);
	Truyendulieu((t%10)+48);
}
void Thongtin(void)
{
	 const unsigned char DA[]={"      DO AN VDK  SV: NGUYEN TUAN VU  13CDT1"};
	 unsigned int i,j;
	 for (i=30;i>=1;i--)
	 {
		 LCD_Vitri(2,1);
		 LCD_Puts("  ***START***");
		 LCD_Vitri(1,1);
		 for (j=0;j<i;j++)
		 {
			 Truyendulieu(DA[45+j-i]);
		 }
		 _delay_ms(2500);
	 }
}
void chedo()
{
				if (inbit(PINB,0)==0)
				{
					while(inbit(PINB,0)==0);
					LCD_Clear();
					LCD_Vitri(1,1);
					LCD_Puts(" Nhiet do: ");
					LCD_Vitri(2,1);
					LCD_Puts(" T dat: ");
					LCD_So(2,9,a);
				}
			 if (inbit(PINB,1)==0)
			 {
				 while(inbit(PINB,1)==0);
				 a++;
				LCD_So(2,9,a);
			 }
			 if (inbit(PINB,2)==0)
			 {
				 while(inbit(PINB,2)==0);
				 a--;
				 if(a==0) a=35;
				 LCD_So(2,9,a);
			 }
}

int main(void)
{   
	DDRB=0xf8;
	PORTB=0xff;
	setb(ADMUX,REFS0);
	setb(ADMUX,MUX1);
	setb(ADCSRA,ADEN);
	setb(ADCSRA,ADPS1);
	setb(ADCSRA,ADPS2);
	LCD_Init(); 
	Thongtin();
	while(1)
	{		  
		       temp();
		       _delay_ms(100);
			   chedo();
			    if (t> a)
			    {
				    setb(PORTC,1);
				    clb(PORTB,4);
				    setb(PORTB,3);
				    _delay_ms(3000);
				    setb(PORTB,4);
			    }
			    else
			    {
				    clb(PORTC,1);
				    clb(PORTB,3);
				    setb(PORTB,4);
				    _delay_ms(500);
				    setb(PORTB,3);
			    }
		//TODO:: Please write your application code
	}
}

