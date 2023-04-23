/*
 * SDCARD.c
 *
 * Created: 3/15/2016 8:18:32 PM
 *  Author: Administrator
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) thanhghi=(thanhghi^(1<<bit))

#include "E:\AVR\Header\mySDCARD.h"
#include "E:\AVR\Header\myUART.h"
//#include "E:\AVR\Header\myLCD.h"

//tao 1 stream ten mystdout thuoc loai FILE (FILE ao), connect voi ham uart_char_tx
static FILE uartstd= FDEV_SETUP_STREAM(putChar_UART, NULL,_FDEV_SETUP_WRITE);
//--------------------------------------------------------------

unsigned char buff[512], tchr; //bo dem phat va nhan
//unsigned char	hocavr[]="##     ##  #######   ######     ###    ##     ## ########       ######   #######  ##     ##\n##     ## ##     ## ##    ##   ## ##   ##     ## ##     ##     ##    ## ##     ## ###   ###\n##     ## ##     ## ##        ##   ##  ##     ## ##     ##     ##       ##     ## #### ####\n######### ##     ## ##       ##     ## ##     ## ########      ##       ##     ## ## ### ##\n##     ## ##     ## ##       #########  ##   ##  ##   ##       ##       ##     ## ##     ##\n##     ## ##     ## ##    ## ##     ##   ## ##   ##    ##  ### ##    ## ##     ## ##     ##\n##     ##  #######   ######  ##     ##    ###    ##     ## ###  ######   #######  ##     ##\n";
char hocavr[]="vanbaat\n";
int main(void)
{
	init_UART(); //khoi dong uart, baud=38400
	DDRC=0xFF; PORTC=0x00;
	
	//dien vao bo dem de ghi vao mmc--------------------------
	char i, j;
	char Res;
	
	Res=mmc_init();		//da bao gom khoi dong SPI ca mmc
	PORTC++; //xong buoc 1, khoi dong card thanh cong
	if(Res !=0) goto Finish;
	
	//--------------------Phan ghi du lieu---------------------------------------
	//Ghi sector 1
	unsigned char *teststring = "Welcome to WWW.HOCAVR.COM. This is the demo of MMC/SD card manipulation\n\n";
	Res=mmc_writeblock(1,teststring); //ghi vao sector 1
	PORTC++; //xong buoc 2, ghi vao sector 1 thanh cong
	if(Res !=0) goto Finish;
	
	//Ghi sector 2
	teststring ="Phan noi dung \n";
	//teststring ="Phan noi dung nay duoc luu o section thu 2 cua card: \nAtmel AVR \nFrom Wikipedia, the free encyclopedia \nThe AVR is a modified Harvard architecture 8-bit RISC single chip microcontroller \nwhich was developed by Atmel in 1996. The AVR was one of the first microcontroller families \nto use on-chip flash memory for program storage, as opposed to One-Time Programmable ROM, \nEPROM, or EEPROM used by other microcontrollers at the time.\n";
	Res=mmc_writeblock(2,teststring); //ghi vao sector 2
	PORTC++; //xong buoc 3, ghi vao sector 2 thanh cong
	if(Res !=0) goto Finish;
	
	//---ghi hocavr.com vao MMC---------------------------
	tchr=pgm_read_byte(&hocavr[0]);
	for (i=0;tchr>0;i++){
		if (i<512){
			tchr=pgm_read_byte(&hocavr[i]);
			buff[i] = tchr;
		}
		else break;
	}
	Res=mmc_writeblock(3,buff); //ghi vao sector 3
	PORTC++; //xong buoc 4, ghi vao sector 3 thanh cong
	if(Res !=0) goto Finish;
	
	for (j=0;tchr>0;j++){
		if (j<512){
			tchr=pgm_read_byte(&hocavr[i+j]);
			buff[j] = tchr;
		}
		else break;
	}
	Res=mmc_writeblock(4,buff); //ghi vao sector 4
	PORTC++; //xong buoc 5, ghi vao sector 4 thanh cong
	if(Res !=0) goto Finish;
	
	
	//-------------------Phan doc du lieu----------------------------------------
	//Doc sector 1
	Res=mmc_readblock(1,buff);   //doc sector 1
	PORTC++; //xong buoc 6, doc sector 1 thanh cong
	fprintf(&uartstd,buff); //in toan bo chuoi doc ve len Terminal
	
	//Doc sector 2
	Res=mmc_readblock(2,buff);   //doc sector 2
	PORTC++; //xong buoc 7, doc sector 2 thanh cong
	//fprintf(&uartstd,"\n\n"); //xuong 2 dong
	fprintf(&uartstd,buff); //in toan bo chuoi doc ve len Terminal
	
	//Doc sector 3
	Res=mmc_readblock(3,buff);   //doc sector 3
	PORTC++; //xong buoc 8, doc sector 3 thanh cong
	fprintf(&uartstd,"\n\n"); //xuong  2 dong
	fprintf(&uartstd,buff); //in toan bo chuoi doc ve len Terminal
	//Doc sector 4
	Res=mmc_readblock(4,buff);   //doc sector 4
	PORTC++; //xong buoc 9, doc sector 4 thanh cong
	fprintf(&uartstd,buff); //in toan bo chuoi doc ve len Terminal

	Finish:
	PORTC |=(Res<<4); //hien thi trang thai MMC ra LED 2
	
	while(1)
	{
		
	}
	return 0;
}