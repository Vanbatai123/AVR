/*
 * UART.c
 *
 * Created: 1/17/2016 6:26:40 PM
 *  Author: Administrator
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "E:\AVR\Header\myLCD.h"
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
int data;
void putChar_UART(int kitu)
{
	while (inbit(UCSRA,UDRE)==0); //cho den khi bit UDRE=1 Neu = 1 thi viet code vao
	UDR=kitu;
}
void print_UART(char* str)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; str[i]!=0; i++) 
	putChar_UART(str[i]);
}
void vietso_UART(int num)
{
	char dis[5];
	sprintf(dis, "%i", num);
	print_UART(dis);
}
int main(void)
{
					//set baud, 57.6k ung voi f=8Mhz, xem bang 70 trang 165, Atmega32 datasheet
	UBRRH=0;		//set toc do truyen baud
	UBRRL=51;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	//cho phep ca 2 qua trinh nhan va//truyen, va cho  phep ngat sau khi nhan xong
	
	print_UART("SILENCER");
	sei();
	while(1)
	{
		/*for (int i=32;i<128;i++)
		{
			putChar_UART(i);
			_delay_ms(500);
		}*/
	}
}
 ISR(USART_RXC_vect)
 {
	data=UDR;
	putChar_UART(data);
	_delay_ms(50);
 }
