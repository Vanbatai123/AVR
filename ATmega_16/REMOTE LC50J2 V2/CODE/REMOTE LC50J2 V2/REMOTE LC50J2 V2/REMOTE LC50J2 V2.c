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

int x=-1,n,y;
unsigned long int dataa;

int check_start()
{
	if(inbit(DATA_PIN,DATA)==0)						 // Neu co canh xuong cua tin hieu
	{
		_delay_us(500);										 // Delay de chong nhieu
		if(inbit(DATA_PIN,DATA)==0)					 // Neu dung la co canh xuong cua tin hieu
		{
			for(int i=0;i<45;i++)								 // Chia nho xung START thanh 40 doan, moi doan dai 200us
			{
				_delay_us(200);
				if(inbit(DATA_PIN,DATA)==1)
				return 0;									 // Neu tin hieu len muc cao (1) --> khong phai xung START
			}
			while(inbit(DATA_PIN,DATA)==1);
			return 1;										 // Dung la xung start
		}
		
	}
}
//  Ham doc ma cua cac kenh
unsigned long int infr_sensor()
{
	unsigned char i;
	unsigned int Data_Infr;
	Data_Infr=0;
	if(check_start())                  // Neu dung xung START
	{
		while(inbit(DATA_PIN,DATA)==1);          // Cho het bit 1 tiep theo
		while(inbit(DATA_PIN,DATA)==0);          // Cho het bit 0 tiep theo
		for(i=0;i<24;i++)             // Lay 24 bit du lieu (bao gom 8 bit 0 va 8 bit 1 dau tien sau xung start)
		{
			//while(inbit(DATA_PORT,DATA_PIN)==0);
			_delay_us(750);           // Delay 750us de kiem tra bit
			if(inbit(DATA_PIN,DATA)==0) Data_Infr=Data_Infr*2 + 1;
			if(inbit(DATA_PIN,DATA)==1) Data_Infr=Data_Infr*2;
			while(inbit(DATA_PIN,DATA)==1);     // cho canh len
			while(inbit(DATA_PIN,DATA)==0);     // cho canh len
		}
		_delay_ms(600);                // Cho het tin hieu (chong nhieu)
	}
	return Data_Infr;                  // Gia tri ma kenh tra ve
}
int main(void)
{
	
	DDRA=0xff;
	DDRC=0xff;
	DDRB=0xff;
	DDRD=0x00;
	PORTD=0xff;
	PORTB=0x00;
	/*
	//UART
	UBRRH=0;		//set toc do truyen baud
	UBRRL=51;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	//NGAT NGOAI
	setb(PORTD,0);
	MCUCR=(1<<ISC01)|(1<<ISC11);
	GICR=(1<<INT0)|(1<<INT1);
	sei();*/
	while(1)
	{
		dataa = infr_sensor();    // Gan ma cua kenh vao bien Data 

									//Ham hien thi gia tri ma cua cac kenh len LCD1602
		if(dataa!=0)
		{
			vietso_UART(dataa);

		}
	}
}
/*ISR(INT0_vect)
{
	x++;
	while(inbit(DATA_PIN,DATA)==1);
	//setb(TCCR1B,CS10);
	TCCR1B=(1<<CS11);// CS02=1, CS01=1, CS00=0: xung nhip tu  chan T0, down
	TCNT1=0;
	//setb(TCCR1B,CS12);
	while(inbit(DATA_PIN,DATA)==0);
	n=TCNT1;
	if (n<mang[x])
	{
		y=y&0;
	}
	if (x==0)
	{
		vietso_UART(y);
		putChar_UART(' ');
		x=-1;
	}
	
	if (n>9000)
	{
		//vietso_UART(n);
		putChar_UART('-');
		print_UART("la xung start");
		clrb(PORTA,0);
		
	}
}
*/