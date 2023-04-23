/*
 * SPI.c
 *
 * Created: 2/10/2016 9:23:48 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
//#include "E:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) thanhghi=(thanhghi^(1<<bit))

//dinh nghia cac duong giao tiep SPI tren AVR, phu thuoc cau truc chan cua tung chip
#define SPI_PORT	PORTB
#define SPI_DDR		DDRB
#define	SCK_PIN		7
#define	MISO_PIN	6
#define	MOSI_PIN	5

#define ADDRESS_PORT	PORTB
#define ADDRESS_DDR		DDRB
#define Slave(i)		i	 //i la thu tu chan tren AVR Master dieu khien cac Slave

int wData[3]={0, 80, 160}; //wData: du lieu de send

//Khoi dong SPI o che do Master
void SPI_MasterInit(void)
{
	SPI_DDR |=(1<<SCK_PIN)|(1<<MOSI_PIN);
	SPI_PORT|=(1<<MISO_PIN); //dien tro keo len cho chan MISO
	SPCR=(1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
	//SPIE: interrupt, SPE: enable, MSTR: Master mode, CPHA: Phase, SPR1:0: prescaler=128
	//Setup cho cac chan dieu khien dia chi Slave
	ADDRESS_DDR |=(1<<Slave(2))|(1<<Slave(1))|(1<<Slave(0));
	ADDRESS_PORT|=(1<<Slave(2))|(1<<Slave(1))|(1<<Slave(0)); //keo len muc cao de disable Slavers
}

//Send data, 1 byte, qua SPI
void SPI_Transmit(int i, int data)
{
	clrb(ADDRESS_PORT, Slave(i));
	SPDR=data;
	while (inbit(SPSR,SPIF)==0); //cho den khi bit SPIF duoc set, qua trinh truyen ket thuc
	setb(ADDRESS_PORT, Slave(i));
}

//chuong trinh chinh
int main()
{
	SPI_MasterInit(); //khoi dong SPI Master
	init_LCD();
	_delay_ms(100);	  //cho 1 khoang ngan de Slave kip khoi dong
	while(1)
	{
		SPI_Transmit(0, wData[0]++);
		if (wData[0]>80) 
		{
			wData[0]=0;
		} 
		_delay_ms(10);	//to Salve 0
		SPI_Transmit(1, wData[1]++);
		if (wData[1]>160)
		{
			wData[1]=80;
		}
		_delay_ms(10);	//to Salve 1
		SPI_Transmit(2, wData[2]++);
		if (wData[2]>240)
		{
			wData[2]=160;
		}
		_delay_ms(10);	//to Salve 2
		_delay_ms(50);
	}
}

