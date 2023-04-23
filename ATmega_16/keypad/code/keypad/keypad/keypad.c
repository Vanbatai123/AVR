/*
 * keypad.c
 *
 * Created: 11/16/2015 8:55:12 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit

#define KEYPAD_DDR DDRD
#define KEYPAD_PORT PORTD
#define KEYPAD_PIN PIND

//int *a;
char  keypad[]={4,4,5,6,7};
char  button[][4]={
					7,8,9,15,
					4,5,6,14,
					1,2,3,13,
					10,0,11,12
					};
void keypadfunc()
{	
	int i,j;
	for(i=0;i<4;i++)
	{
		clrb(KEYPAD_PORT,i);
		for (j=0;j<4;j++)
		{
			if (inbit(KEYPAD_PIN,(j+4))==0)
			{
				vietso_LCD(button[i][j]);
				_delay_ms(300);
			}
		}
		setb(KEYPAD_PORT,i);
	}
}
int main(void)
{
	KEYPAD_PORT=0xF0;
	KEYPAD_DDR=0x0F;
	int a=0,dem=0,pass=1234;
	init_LCD();
	clr_LCD();
	   while(1)
    {
		int i,j;
		for(i=0;i<4;i++)
		{
			clrb(KEYPAD_PORT,i);
			for (j=0;j<4;j++)
			{
				if (inbit(KEYPAD_PIN,(j+4))==0)
				{
					dem++;
					putChar_LCD('*');
					//vietso_LCD(button[i][j]);
					_delay_ms(300);
					if ((button[i][j]<10)&&(button[i][j]>-1))
					{
						a=10*a+button[i][j];
					}
				}
			}
			setb(KEYPAD_PORT,i);
		}
		if (dem==4)
		{
			if (a==pass)
			{
				move_LCD(2,1);
				print_LCD("DUNG ROI");
			}
			else
			{
				move_LCD(2,1);
				print_LCD("SAI ROI");
			}
			break;
		}
    }
}