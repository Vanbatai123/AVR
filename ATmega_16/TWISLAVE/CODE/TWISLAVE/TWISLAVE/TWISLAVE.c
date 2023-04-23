/*
 * TWISLAVE.c
 *
 * Created: 2/2/2016 8:10:14 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
# define F_CPU 8000000UL
#include <util/delay.h>
int  i,j,a,dataz[]={1,2,3,4,5,6,7,8,9};
#include <stdio.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#include "E:\AVR\Header\myTWI.h"
#include "E:\AVR\Header\myLCD.h"

//chuong trinh chinh
int main(void)
{
	init_LCD();
	TWI_init_slave(0x20);
	//sei();
	//int write_data[50]={1,2,3,4,5,6,7,8,9};
	while (1)
	{
		/*TWI_match_read_slave();
		TWI_read_slave();
		move_LCD(1,1);
		vietso(data_received[i]);
		print_LCD("  ");*/
		
		//TWI_match_write_slave();
		//TWI_write_slave();
		move_LCD(1,1);
		vietso(write_data);
		print_LCD(" ");
	}
}
/*ISR(TWI_vect)
{
	switch(TWSR)
	{
		case 0x60:
		j++;
		_delay_us(30);
		// Get acknowlegement, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWIE)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));				// Wait for TWINT flag
	
		TWCR= (1<<TWEA)|(1<<TWEN)|(1<<TWIE)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));			  // Wait for TWINT flag			
		data_received[j]=TWDR;					// Wait for acknowledgement
	}
}*/