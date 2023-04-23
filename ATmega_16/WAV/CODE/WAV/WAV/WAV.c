#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
//#include "E:\AVR\Header\myLCD.h"
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#include <stdint.h>
#include <avr/pgmspace.h>
#include "C:\Users\Administrator\Downloads\Compressed\BonPetit\BonPetit\diskio.h"
#include "C:\Users\Administrator\Downloads\Compressed\BonPetit\BonPetit\pff.h"

unsigned char buffer[512];   /* ????? ? ??????? ?????????? ???? ? ?????? */
volatile unsigned int count; //??????? ????????????? ??????

void timer2_comp_isr(void) //?????????? ? ??????? ????????????? ????????
{
	OCR1A = buffer[count]; //??????? ???? ?? ???????
	if (++count >= 512) //??????????? ???????
	count = 0;  //???? 512 ????????
}

void main(void)
{
	unsigned int br;         /* ??????? ??????/?????? ????? */
	unsigned char buf = 0;   //?????????? ???????????? ????? ????? ?????? ????????
	FATFS fs;         /* ??????? ??????? (file system object) ??? ?????????? ?????? */
	
	PORTD=0x00;
	DDRD=(1<<PD5);  //??????? ????? ocr1a
	
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 8000,000 kHz
	// Mode: Fast PWM top=0x00FF
	// OC1A output: Non-Inv.
	TCCR1A=0x81;
	TCCR1B=0x09;
	TCNT1=0x00;
	OCR1A=0x00;
	
	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 250,000 kHz
	// Mode: CTC top=OCR2
	TCCR2=0x0B;
	TCNT2=0x00;
	//OCR2=0x1E; //????????? ???????? ????????? ??? 8???
	OCR2=0xA;    //??? 22???
	
	sei();
	// Timer(s)/Counter(s) Interrupt(s) initialization
	
	
	if(disk_initialize()==0) //?????????????? ??????
	{
		pf_mount(&fs);  //????????? ???????? ???????
		pf_open("1.wav");  //????????? ?????
		pf_lseek(44);     //?????????? ????????? ?? 44
		pf_read(buffer, 512,&br); //? ?????? ??? ??????????? ????? 512????
		TIMSK=0x80; //??????? ?????
		while(1)
		{
			if(!buf && count>255) //???? ?????????????? ?????? 255 ????,
			{
				pf_read(&buffer[0], 256,&br);//?? ?????? ? ?????? ???????? ?????? ???? ? ??????
				buf=1;
				if (br < 256) //???? ????? ?? ???????? 256 ???????? ?????? ????? ?????
				break;
			}
			if(buf && count<256)
			{
				pf_read(&buffer[256], 256,&br); // ?????? ?? ?????? ????? ?????? ? ??????
				buf = 0;
				if (br < 256)
				break;
			}
			
		}
		TIMSK = 0x00; //?????? ???
		pf_mount(0x00); //??????????? ???
	}
	
	while (1)
	{
		
		
	}
}