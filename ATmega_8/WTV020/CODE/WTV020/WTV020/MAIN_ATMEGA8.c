/*
 * WTV020.c
 *
 * Created: 2/10/2017 7:14:53 PM
 *  Author: Van_BasTai
 */ 


#include "mySTART.h"

#define WTV020_PORT	PORTD
#define WTV020_DDR	DDRD
#define WTV020_PIN	PIND

#define	RESET		0
#define	CLK			1
#define	DATA		2
#define	BUSY		3

#define HI_RESET	setb(WTV020_PORT, RESET)
#define LOW_RESET	clrb(WTV020_PORT, RESET)

#define HI_CLK		setb(WTV020_PORT, CLK)
#define LOW_CLK		clrb(WTV020_PORT, CLK)

#define HI_DATA		setb(WTV020_PORT, DATA)
#define LOW_DATA	clrb(WTV020_PORT, DATA)

#define PAUSE		0xFFFE
#define STOP		0xFFFF

#define VOL_MAX		0xFFF7
#define VOL_MIN		0xFFF0

char song_number = 7;
unsigned int volume;
//------------------RESET---------------------
void WTV020_reset()
{
	LOW_RESET;
	_delay_ms(5);
	HI_RESET;
	_delay_ms(300);
}
//------------------SEND_DATA------------------
void WTV020_Send_data(unsigned int command)
{
	LOW_CLK;
	_delay_ms(2);
	for (unsigned int mask = 0x8000; mask > 0; mask >>= 1) 
	{
		LOW_CLK;  //Clock low level pulse.
		_delay_us(50);
		if (command & mask) //Write data setup.
			HI_DATA;
		else				//Write data hold.
			LOW_DATA;
		_delay_us(50);
		HI_CLK;//Clock high level pulse.
		_delay_us(100);
		if (mask>0x0001)//Stop bit high level pulse.
			_delay_ms(2);
	}
	_delay_ms(20);//Busy active high from last data bit latch.
}
//------------------------Phat ca bai hat------------------------------------//
void WTV020_FullSong(unsigned int song_number)
{
	while (inbit(WTV020_PIN, BUSY) == 1);
	WTV020_Send_data(song_number);
	while (inbit(WTV020_PIN, BUSY) == 1);
	
}
//---------------------Phat mot phan bai hat---------------------------------//
void WTV020_PartSong(unsigned int song_number)
{
	//while (inbit(WTV020_PIN, BUSY) == 1);
	WTV020_Send_data(song_number);
}
//-----------------------------Pause----------------------------------------//
void WTV020_Pause()
{
	WTV020_Send_data(PAUSE);
}
//-----------------------------Play----------------------------------------//
void WTV020_Play()
{
	WTV020_Send_data(PAUSE);
}
//-----------------------------Stop----------------------------------------//
void WTV020_Stop()
{
	WTV020_Send_data(STOP);
}
//--------------------------Volume_Up-------------------------------------//
void WTV020_Volume_Up()
{
	if (volume < 7)
		volume++;
	WTV020_Send_data(VOL_MIN + volume);
}
//-------------------------Volume_Down------------------------------------//
void WTV020_Volume_Down()
{
	if (volume > 0)
		volume--;
	WTV020_Send_data(VOL_MIN + volume);
}
int main(void)
{
	WTV020_PORT = 0xFF;
	WTV020_DDR = 0x07;               
	WTV020_reset();
	
	while(1)
	{	
		if (inbit(WTV020_PIN, 4) == 0)
		{
			song_number++;
			_delay_ms(300);
			WTV020_PartSong(song_number);
			
		} 
		else if (inbit(WTV020_PIN, 5) == 0)
		{
			song_number--;
			_delay_ms(300);
			WTV020_PartSong(song_number);
			//WTV020_Send_data(0xFFF3);
		}
		WTV020_Send_data(0xFFF4);
	}
}
