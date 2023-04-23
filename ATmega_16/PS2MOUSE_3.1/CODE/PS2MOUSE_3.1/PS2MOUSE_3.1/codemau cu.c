/*
 * PS2MOUS.c
 *
 * Created: 12/30/2016 5:16:15 PM
 *  Author: Administrator
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myTIMER_1.h"

#define DATA_PS2 0
#define CLK_PS2 3
#define PORT_BJT PORTB
#define PIN_BJT PINB
#define DDR_BJT DDRB
#define PIN_PS2 PIND

int dembit=1,data=0;

void host_send(int byte)
{
	clrb(GICR,INT1);
	setb(PORT_BJT,CLK_PS2);//CLK=0
	_delay_us(200);
	setb(PORT_BJT,DATA_PS2);//data=0
	clrb(PORT_BJT,CLK_PS2);//clk=1
	while(inbit(PIN_PS2,CLK_PS2)==1);
	for(int y=0;y<7;y++)
	{
		for (int x=0;x<8;x++)
		{
			if ((byte>>x)&0x01)
				setb(PORT_BJT,DATA_PS2);
			else
				clrb(PORT_BJT,DATA_PS2);
		}
		while(inbit(PIN_PS2,CLK_PS2)==0);
		while(inbit(PIN_PS2,CLK_PS2)==1);
	}
	clrb(PORT_BJT,DATA_PS2);
	while(inbit(PIN_PS2,DATA_PS2)==1);
	while(inbit(PIN_PS2,CLK_PS2)==1);
	while((inbit(PIN_PS2,CLK_PS2)==0)&&(inbit(PIN_PS2,DATA_PS2)==0));
	setb(GICR,INT1);
}
int main(void)
{
	_delay_ms(500);
	init_LCD();
	vietso_LCD(111);
	DDR_BJT=0xff;
	PORT_BJT=0x00;
	DDRD=0x00;
	MCUCR=(1<<ISC01)|(1<<ISC11);
	GICR=(1<<INT0)|(1<<INT1);
	sei();
	while(1)
	{
		host_send(0xAA);
		//vietso_LCD(2);
		_delay_ms(1000);
	}
	
}
ISR(INT1_vect)
{
	_delay_us(20);
	if(dembit>1 && dembit<10) 
	{
		data=data>>1;
		if(inbit(PIN_PS2,DATA_PS2)==1)
			data=data+0x80;
	}
	else if (dembit==11)
	{
		move_LCD(1,1);
		vietso_LCD(data);
		data=0;
		dembit=0;
		clrb(GICR,INT1);
	}
	dembit++;	
}