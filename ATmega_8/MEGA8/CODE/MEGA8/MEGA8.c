/*
 * mega8_wav.c
 *
 *  Created on: 11.07.2013
 *      Author: Apparatchik
 */
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))

#include <stdio.h>
#include "E:\AVR\Header\myUART.h"
#include "diskio.h"
#include "pff.c"
#include "integer.h"
#include "mmc.c"
#define LED_ON clrb(PORTB,0);
#define LED_OFF setb(PORTB,0);

uint8_t wave_play(char *FileName);

uint8_t DAC_Buff[512];                   //буфе? данны? ЦАПа
volatile uint16_t count_buff;
FATFS fs;           //понадоби?ся для ?або?ы с диско?
FRESULT res;                        //для возв?ащае?о?о функция?и ?езуль?а?а
DIR dir; // объек? ка?ало?
FILINFO fno; // инфо??ационная с??ук?у?а файла

// Timer 2 output compare A interrupt service routine
ISR (TIMER2_COMP_vect) {
	OCR1A = DAC_Buff[count_buff];
	if (++count_buff >= 512)
		count_buff = 0;
}

int  main(void)
{
	init_UART();
	// Input/Output Ports initialization
    print_UART("1\r");
	DDRB = 0xFF;
	DDRD=0XFF;
	PORTB=PORTD=0X00;
	// Timer/Counter 1 initialization
	// Clock value: 16000,000 kHz
	// Mode: Fast PWM top=00FFh
	TCCR1A=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B=(1<<WGM12)|(1<<WGM13)|(1<<CS10);
	// Mode: Fast PWM top=00FFh
	TCCR1A=0x81; // OC1A output: Non-Inv.
	//	TCCR1A=0xC1; // OC1A output: Inverted
	TCCR1B=0x09;

	// Timer/Counter 2 initialization
	// Mode: CTC top=OCR2
	//	TCCR2=0x0C; // Clock value: 250,000 kHz
	TCCR2=0x0A; // Clock value: 2000,000 kHz
	//	OCR2=0x07; // 8 - 1 = 7 для час?о?ы диск?и?изации 31250 ?ц
	OCR2=0x2C; // 45 - 1 = 44 для час?о?ы диск?и?изации 44100 ?ц

	// Analog Comparator: Off
	//ACSR = 0x80;

	LED_ON;
	_delay_ms (1000);
	LED_OFF;
	_delay_ms(1000);

	sei();

	BYTE status = disk_initialize(); //инициализи?уе? ка??очку

	if (status) 
	{
		while (status) 
		{
			print_UART("WAIT\r");
			LED_ON;
			_delay_ms(250);
			LED_OFF;
			_delay_ms(250);
			status--;
		}
	} 
	else 
	{
		LED_ON;

//		if (!(pf_opendir(&dir, "/"))) { //О?к?ывае? ди?ек?о?и?
//
//			do {
//				if (pf_readdir(&dir, &fno) == FR_OK) {
//					if (!(fno.fattrib & AM_DIR) && fno.fname[0] != 0) {	//Если объек? не ди?ек?о?ия
//						wave_play(fno.fname);
//					}
//				}
//			} while (fno.fname[0] != 0);
//
////			while (pf_readdir(&dir, &fno) == FR_OK && fno.fname[0] != 0) {
////				if (!(fno.fattrib & AM_DIR)) {	//Если объек? не ди?ек?о?ия
////					wave_play(fno.fname);
////				}
////			}
//		}

		print_UART("2\r");
		wave_play ("22.wav");
		print_UART("3\r");
		LED_OFF;
	}

	_delay_ms(1000);

	while (1) 
	{
		//wave_play ("ss.wav");
		print_UART("END\r");
		_delay_ms(100);
		LED_OFF;_delay_ms(100);LED_ON;
	}
}

//*********************************************************************************************
//function восп?оизвес?и wave-файл                                                           //
//argument и?я файла                                                                         //
//result   0 - все но??ально, иначе - ошибка                                                 //
//*********************************************************************************************
uint8_t wave_play(char *FileName) 
{
	WORD cnt;                              //количес?во ?еально п?очи?анны? бай?
	uint8_t buf = 0;

	pf_mount(&fs);           //?он?и?ова?ь диск

	res=pf_open(FileName); //О?к?ое? файл
	if (res)
		return 1;

//	res=pf_lseek (0x2C); //пе?е?ес?и?ь указа?ель на начало полезны? данны?
//	if (res)
//		return 2;
//-------------------------------------------------------------------------------------
	res = pf_read (&DAC_Buff[0], 64, &cnt);  //за??узи?ь пе?вые 64 бай?
	if (res || cnt != 64)
		return 4;
	else {
		  //бай?ы 0..3 - должен бы?ь ?а?ке? "RIFF"
		  if(DAC_Buff[0]!='R' || DAC_Buff[1]!='I' || DAC_Buff[2]!='F' || DAC_Buff[3]!='F')     res = 2;

		  //бай?ы 8..11 - должен бы?ь ?а?ке? "WAVE"
		  if(DAC_Buff[8]!='W' || DAC_Buff[9]!='A' || DAC_Buff[10]!='V' || DAC_Buff[11]!='E')   res = 2;

		  //бай?ы 12..15 - должен бы?ь ?а?ке? "fmt "
		  if(DAC_Buff[12]!='f' || DAC_Buff[13]!='m' || DAC_Buff[14]!='t' || DAC_Buff[15]!=' ') res = 2;

		  //бай?ы 20,21  - 1 и 0
		  if(DAC_Buff[20]!=1 || DAC_Buff[21]!=0) res = 2;

//		  //вычисли?ь ?аз?е? чанка "fmt "
//		  i = DAC_Buff[16] | (DAC_Buff[17]<<8) | (DAC_Buff[18]<<16) | (DAC_Buff[19]<<24);
//
//		  //после чанка "fmt " должен следова?ь чанк "data"
//		  if(DAC_Buff[20+i]!='d' || DAC_Buff[21+i]!='a' || DAC_Buff[22+i]!='t' || DAC_Buff[23+i]!='a') res = 2;

		  if (res == 2) {
			pf_mount(0x00);
			return 5;                   //ошибка фо??а?а файла
		}
	}
//--------------------------------------------------------------------------------------------
	res = pf_read(&DAC_Buff[0], 512, &cnt);//Счи?ае? пе?вые 512 бай?
	if (res)
		return 3;

	TIMSK = 0x80;                   //запус?и?ь п?еоб?азование

	/*                        восп?оизведение                              */
	while (1)
	{
		if (!buf && count_buff > 255) { //жде? освобождение пе?вой час?и буфе?а
			pf_read(&DAC_Buff[0], 256, &cnt);    //за??узи?ь ее данны?и
			buf = 1;
			if (cnt < 256)
				break;                        //если конец файла
		}

		if (buf && count_buff < 256) { //жде? освобождение в?о?ой час?и буфе?а
			pf_read(&DAC_Buff[256], 256, &cnt);  //за??узи?ь ее данны?и
			buf = 0;
			if (cnt < 256)
				break;                        //если конец файла
		}
	}

	TIMSK = 0x00;                  //ос?анови?ь п?еоб?азование

	res=pf_mount(0x00); //Де?он?и?уе?t FAT
	return 0;                                   //успешное заве?шение ф-ии
}