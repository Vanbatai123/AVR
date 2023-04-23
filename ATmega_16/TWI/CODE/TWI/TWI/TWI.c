/*
 * TWI.c
 *
 * Created: 1/30/2016 7:17:54 AM
 *  Author: Administrator
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#include "E:\AVR\Header\myLCD.h"
#include <avr/interrupt.h>
int i=0;
int data[]={2,3,5,1,7,9,1,3,5,7,9,211,44,55,66,77,88,99};
int a,address=0x20;
#include "E:\AVR\Header\myTWI.h"

void TWI_Master_Read(int add,int data[])
{
	TWI_start();
	TWI_read_address(add+read);
	TWI_read_data();
	TWI_stop();
	_delay_ms(1);
}
int main(void)
{
	init_LCD();
	TWI_init_master();
	//thiet lap ngat ngoai
	PORTD=0xFF;	//pull-up
	DDRD=0x00;
	//MCUCR	|=(1<<ISC01); // falling edge
	//GICR    |=(1<<INT0);  // enable INT0
	//----------------------------------
	//sei(); //global interrupt
	while (1)
	{
		
		/*move_LCD(1,1);
		TWI_Master_Send(0x20,data);
		print_LCD("Data Sent: ");
		vietso(data[i]);
		print_LCD(" ");
		i++;
		_delay_ms(500);
		// dia chi, data, mode*/
		
		move_LCD(1,1);
		TWI_Master_Read(0x20,data);// dia chi, data, mode
		vietso(data_received[0]);
		
	}
}
/*ISR (INT0_vect)
{
	
	
}*/
