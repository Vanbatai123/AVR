/*
 * INPUT_CAPTURE.c
 *
 * Created: 3/13/2016 11:46:37 AM
 *  Author: Administrator
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#include "E:\AVR\Header\myUART.h"
#include "E:\AVR\Header\myLCD.h"
int data;
volatile uint32_t 	period=0, duty=0, pre_val=0,over_step=0;
double T,F,D, t1,t2, Scale1, Scale2 ;




int main(void)
{
	//--------------------UART
					//set baud, 57.6k ung voi f=8Mhz, xem bang 70 trang 165, Atmega32 datasheet
	UBRRH=0;		//set toc do truyen baud
	UBRRL=51;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);	//cho phep ca 2 qua trinh nhan va//truyen, va cho  phep ngat sau khi nhan xong
	
	sei();
	//print_UART("SILENCER");
	//-----------------------------------------
	//------------------ICP
	Scale1=F_CPU/1000000; //F_CPU duoc dinh nghia trong Makefile
	Scale2=F_CPU/1000;
	
	init_LCD();
	clr_LCD();
	print_LCD("ww.hocavr.com !");
	clr_LCD();
	print_LCD(" T( s)     f( )");
	
	clrb(DDRD, 6);  //chan ICP1(PD6) la input
	setb(PORTD, 6); //keo chan ICP1 len cao bang dien tro noi
	
	TCNT1=0;
	TCCR1B=(1<<CS10);	   // CS12=0, CS11=0, CS10=1: chon Prescaler = 1, input capture canh xuong
	TIMSK= (1<<TICIE1)|(1<<TOIE1);          // cho phep ngat Input Capture, ngat tran
	sei(); 		                              //set bit I cho phep ngat toan cuc
	
	_delay_ms(100);
	
	while(1)
	{
		
		move_LCD(2,1);
		print_LCD("                "); //xoa hang 2 LCD
		t1=(float)period; //chu ky
		if(t1<Scale2)		// < 1000 us	, hien thi T theo  micro giay va F theo kH
		{				
			T=t1/Scale1;	 //tinh bang us
			move_LCD(1,4);
			putChar_LCD('u');
			move_LCD(1,14); 
			putChar_LCD('k');
			//doi so T sang dang chuoi, chua trong disT
		}
		else
		{ //hien thi T theo mili giay va F theo H
			T=t1/Scale2;	//tinh bang ms
			move_LCD(1,4);
			putChar_LCD('m');
			move_LCD(1,14);
			putChar_LCD('H');
			//doi so T sang dang chuoi, chua trong disT
		}
	
		F=1000/T;	//tinh tan so theo chu ky T
		//doi so F sang dang chuoi, chua trong disF
	
		move_LCD(2,2);
		vietso_LCD(T); //hien thi chu ky  len LCD
		move_LCD(2,12);
		vietso_LCD(F);//hien thi tan so  len LCD
		_delay_ms(300);
	}
return 0;
}
//trinh phuc vu ngat tran T/C1
ISR (TIMER1_OVF_vect )
{
	over_step+=0xFFFF; //tran bo dem, tang so dem them 0xFFFF
}
//trinh phuc vu ngat Input Capture T/C1
ISR (TIMER1_CAPT_vect)
{
	period= over_step + ICR1-pre_val;	//tinh chu ky xung
	pre_val=ICR1; 						// luu gia tri ICR1 cho lan sau
	over_step=0;
}
