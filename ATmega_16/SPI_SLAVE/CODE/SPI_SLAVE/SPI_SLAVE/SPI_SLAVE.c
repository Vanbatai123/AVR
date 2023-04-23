/*
 * SPI_SLAVE.c
 *
 * Created: 2/10/2016 9:28:14 PM
 *  Author: Administrator
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
#include "E:\AVR\Header\myLCD.h"
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#include <avr/interrupt.h>


//dinh nghia cac duong giao tiep SPI tren AVR, phu thuoc cau truc chan cua tung chip
#define SPI_PORT	PORTB
#define SPI_DDR		DDRB
#define	SCK_PIN		7
#define	MISO_PIN	6
#define	MOSI_PIN	5
#define	SS_PIN		4

int wData=16, rData; //wData: sent data, rData; get Data

//Khoi dong SPI o che do Slave
void SPI_SlaveInit(void)
{        //18
	SPI_DDR |=(1<<MISO_PIN);
	SPI_PORT|=(1<<MOSI_PIN)|(1<<SS_PIN); //dien tro keo len cho chan MOSI va SS
	SPCR=(1<<SPIE)|(1<<SPE)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
	//SPIE: interrupt, SPE: enable, MSTR=0: Slave mode, CPHA: Phase, SPR1:0: prescaler=128
}

//Send data, 1 byte, qua SPI
void SPI_Transmit(int data)
{
	SPDR=data;
	while (inbit(SPSR,SPIF)==0);
}

//chuong trinh chinh
int main()//26
{	 
	sei();
	SPI_SlaveInit();
	init_LCD();
	//clr_LCD();
	while(1)
	{
		//do nothing
	}
}
//trinh ngat SPI, ap dung khi du lieu da duoc nhan
ISR(SPI_STC_vect)
{
	rData=SPDR; //37
	clr_LCD();
	move_LCD(1,1);
	vietso(rData);  //hien thi LCD
}
