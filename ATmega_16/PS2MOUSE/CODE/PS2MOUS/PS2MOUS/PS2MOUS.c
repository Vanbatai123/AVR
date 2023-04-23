/*
 * PS2MOUSE.c
 * Using Interrupt 1
 * Created: 12/30/2016 5:16:15 PM
 *  Author: Van BasTai
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myUART.h"
#include "F:\AVR\Header\myTIMER_1.h"
#include "F:\AVR\Header\myPS2MOUSE_INTRRUPT.h"

char status,mx,my;
int main(void)
{
	init_UART();//khai bao UART
	//vietso_UART(123);
	DDR_BJT=0xff;
	PORT_BJT=0x00;
	DDR_PS2=0X03;
	MCUCR=1<<ISC11;//ngat suon xuong timer 1
	sei();
	
	mouse_init();
	while(1)
	{
		host_send(0xEB);
		while(inbit(GICR,INT1));
		vietso_UART(data);
		space_UART(2);
		setb(GICR,INT1);
		while(inbit(GICR,INT1));
		vietso_UART(data);
		space_UART(2);
		setb(GICR,INT1);
		while(inbit(GICR,INT1));
		vietso_UART(data);
		space_UART(2);
		setb(GICR,INT1);
		while(inbit(GICR,INT1));
		vietso_UART(data);
		newline_UART();
		_delay_ms(400);
		//host_send(0xeb);
		//vietso_UART(3435);
		//host_send(0xf0);_delay_ms(500);
		//vietso_UART(555);
		//newline_UART();
	}//end while(1)
}//edn main
ISR(INT1_vect)
{
	if (dembit>2 && dembit<11) //read 8 bit in 11 bit from mouse
	{
		data=data>>1;
		if(inbit(PIN_PS2,DATA)==1)
			data=data+0x80;
	}//end if
	if (--dembit==0)
	{
		dembit=11;
		clrb(GICR,INT1);	//disable interrupt 1
		UP_CLK;				//CLK=1
		UP_DATA;			//DATA=1
		
	}	
}//end interrupt 1