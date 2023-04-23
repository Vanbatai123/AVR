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

uint8_t DAC_Buff[512];                   //����? �����? ����
volatile uint16_t count_buff;
FATFS fs;           //��������?�� ��� ?���?� � �����?
FRESULT res;                        //��� ����?����?�?� �������?� ?�����?�?�
DIR dir; // �����? ��?���?
FILINFO fno; // ����??�������� �??��?�?� �����

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
	//	OCR2=0x07; // 8 - 1 = 7 ��� ���?�?� ����?�?������ 31250 ?�
	OCR2=0x2C; // 45 - 1 = 44 ��� ���?�?� ����?�?������ 44100 ?�

	// Analog Comparator: Off
	//ACSR = 0x80;

	LED_ON;
	_delay_ms (1000);
	LED_OFF;
	_delay_ms(1000);

	sei();

	BYTE status = disk_initialize(); //����������?��? ��??����

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

//		if (!(pf_opendir(&dir, "/"))) { //�?�?����? ��?��?�?�?
//
//			do {
//				if (pf_readdir(&dir, &fno) == FR_OK) {
//					if (!(fno.fattrib & AM_DIR) && fno.fname[0] != 0) {	//���� �����? �� ��?��?�?��
//						wave_play(fno.fname);
//					}
//				}
//			} while (fno.fname[0] != 0);
//
////			while (pf_readdir(&dir, &fno) == FR_OK && fno.fname[0] != 0) {
////				if (!(fno.fattrib & AM_DIR)) {	//���� �����? �� ��?��?�?��
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
//function ����?������?� wave-����                                                           //
//argument �?� �����                                                                         //
//result   0 - ��� ��??�����, ����� - ������                                                 //
//*********************************************************************************************
uint8_t wave_play(char *FileName) 
{
	WORD cnt;                              //�������?�� ?������ �?���?����? ���?
	uint8_t buf = 0;

	pf_mount(&fs);           //?��?�?���?� ����

	res=pf_open(FileName); //�?�?��? ����
	if (res)
		return 1;

//	res=pf_lseek (0x2C); //��?�?��?�?� �����?��� �� ������ �������? �����?
//	if (res)
//		return 2;
//-------------------------------------------------------------------------------------
	res = pf_read (&DAC_Buff[0], 64, &cnt);  //��??���?� ��?��� 64 ���?
	if (res || cnt != 64)
		return 4;
	else {
		  //���?� 0..3 - ������ ��?� ?�?��? "RIFF"
		  if(DAC_Buff[0]!='R' || DAC_Buff[1]!='I' || DAC_Buff[2]!='F' || DAC_Buff[3]!='F')     res = 2;

		  //���?� 8..11 - ������ ��?� ?�?��? "WAVE"
		  if(DAC_Buff[8]!='W' || DAC_Buff[9]!='A' || DAC_Buff[10]!='V' || DAC_Buff[11]!='E')   res = 2;

		  //���?� 12..15 - ������ ��?� ?�?��? "fmt "
		  if(DAC_Buff[12]!='f' || DAC_Buff[13]!='m' || DAC_Buff[14]!='t' || DAC_Buff[15]!=' ') res = 2;

		  //���?� 20,21  - 1 � 0
		  if(DAC_Buff[20]!=1 || DAC_Buff[21]!=0) res = 2;

//		  //�������?� ?��?�? ����� "fmt "
//		  i = DAC_Buff[16] | (DAC_Buff[17]<<8) | (DAC_Buff[18]<<16) | (DAC_Buff[19]<<24);
//
//		  //����� ����� "fmt " ������ �������?� ���� "data"
//		  if(DAC_Buff[20+i]!='d' || DAC_Buff[21+i]!='a' || DAC_Buff[22+i]!='t' || DAC_Buff[23+i]!='a') res = 2;

		  if (res == 2) {
			pf_mount(0x00);
			return 5;                   //������ ��??�?� �����
		}
	}
//--------------------------------------------------------------------------------------------
	res = pf_read(&DAC_Buff[0], 512, &cnt);//���?��? ��?��� 512 ���?
	if (res)
		return 3;

	TIMSK = 0x80;                   //�����?�?� �?���?��������

	/*                        ����?����������                              */
	while (1)
	{
		if (!buf && count_buff > 255) { //���? ������������ ��?��� ���?� ����?�
			pf_read(&DAC_Buff[0], 256, &cnt);    //��??���?� �� �����?�
			buf = 1;
			if (cnt < 256)
				break;                        //���� ����� �����
		}

		if (buf && count_buff < 256) { //���? ������������ �?�?�� ���?� ����?�
			pf_read(&DAC_Buff[256], 256, &cnt);  //��??���?� �� �����?�
			buf = 0;
			if (cnt < 256)
				break;                        //���� ����� �����
		}
	}

	TIMSK = 0x00;                  //��?�����?� �?���?��������

	res=pf_mount(0x00); //��?��?�?��?t FAT
	return 0;                                   //�������� ����?����� �-��
}