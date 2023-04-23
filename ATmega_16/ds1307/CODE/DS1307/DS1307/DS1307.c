/*
 * lcd.c
 *
 * Created: 10/27/2015 8:11:05 AM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"

#define min 1
#define hou 2
#define dat 3
#define mon 4
#define yea 5
#define BUTTON_PORT PORTB
#define BUTTON_DDR DDRB
#define BUTTON_PIN PINB
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
//Mode: chon che do 12h hoac 24h, Mode nam o bit 6 cua thanh ghi HOURS, 
//Mode=1: 12H, Mode=0: 24H
//AP: bien chi AM hoac PM trong Mode 12h, AP nam o bit 5 cua thang ghi HOURS, 
//AP=1:PM, AP=0: Am		
int tData[7],reg[]={0x00}, Time_count=0; //dinh nghia cac bien tam

	
#include "E:\AVR\Header\myDS1307.h"
									//chuong trinh chinh
int main(void)
{	
	//khoi dong LCD va DS1307-----------------------
	init_LCD();
	init_DS1307();
	clr_LCD();	
	//------------------------------------
	BUTTON_DDR=0X00;
	BUTTON_PORT=0XFF;
	//khoi dong Timer0 lam bo dinh thi 1s------------------------------------------------CHUA HIEU
	TCCR0=(1<<CS02)|(0<<CS01)|(1<<CS00);	//CS02=1, CS01=0, CS00=1: chon Prescaler=1024
	TIMSK=(1<<TOIE0);						//cho phep ngat khi co tran o T/C0
	sei();                     			//set bit I cho phep ngat toan cuc
	//----------------------------------------------------------------
	int dem=0,j=0;
	//khoi dong gia tri ghi vao DS1307---------------------------------------------------
	/*tData[0]=Dec2BCD(Second); 
	tData[1]=Dec2BCD(Minute); 
	if (Mode!=0) tData[2]=Dec2BCD(Hour)|(Mode<<6)|(AP<<5); //mode 12h
	else tData[2]=Dec2BCD(Hour);
	tData[3]=Dec2BCD(Day);
	tData[4]=Dec2BCD(Date);
	tData[5]=Dec2BCD(Month); 
	tData[6]=Dec2BCD(Year);	*/
	
	ghi(reg);
	doc();
	
	Display(); // hien thi ket qua len LCD	
	while(1)
	{	
		move_LCD(2,15);
		if(inbit(BUTTON_PIN,0)==0)
		{
			dem++;
			_delay_ms(260);
		}
		if ((dem%6==min))
		{
			
			print_LCD("M");
			if (inbit(BUTTON_PIN,1)==0)
			{
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Minute++;
					if (Minute==60)
					{
						Minute=0;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
			if ((inbit(BUTTON_PIN,2)==0))
			{
						
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Minute--;
					if (Minute==-1)
					{
						Minute=59;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
		}
		else if ((dem%6==hou))
		{
			
			print_LCD("H");
			if (inbit(BUTTON_PIN,1)==0)
			{
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Hour++;
					if (Hour==24)
					{
						Hour=0;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
			if ((inbit(BUTTON_PIN,2)==0))
			{
				
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Hour--;
					if (Hour==-1)
					{
						Hour=23;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
		}
		else if ((dem%6==dat))
		{
			
			print_LCD("D");
			if (inbit(BUTTON_PIN,1)==0)
			{
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Date++;
					if (Date==32)
					{
						Date=1;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
			if ((inbit(BUTTON_PIN,2)==0))
			{
				
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Date--;
					if (Date==0)
					{
						Date=31;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
		}
		else if ((dem%6==mon))
		{
			
			print_LCD("m");
			if (inbit(BUTTON_PIN,1)==0)
			{
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Month++;
					if (Month==13)
					{
						Month=1;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
			if ((inbit(BUTTON_PIN,2)==0))
			{
				
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Month--;
					if (Month==0)
					{
						Month=12;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
		}
		else if ((dem%6==yea))
		{
			
			print_LCD("Y");
			if (inbit(BUTTON_PIN,1)==0)
			{
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Year++;
					if (Year==100)
					{
						Year=0;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
			if ((inbit(BUTTON_PIN,2)==0))
			{
				
				j++;
				_delay_ms(15);
				if (j%15==0)
				{
					Year--;
					if (Year==-1)
					{
						Year=99;
					}
					chuanbi();
					goidulieu(0x00,tData);
				}
			}
		}
		else
		print_LCD(" ");
	}
}
ISR (TIMER0_OVF_vect )
{
	Time_count++;
	if(Time_count>=10)
	{
		ghi(reg);
		doc();
		Display();
		Time_count=0;
	}
}
