/*
 * IR_REMOTE.c
 *
 * Created: 5/25/2017 11:14:53 AM
 *  Author: Van_BasTai
 */ 

#define DATA_DDR DDRC
#define DATA_PORT PORTC
#define DATA_PIN PINC
#define DATA 0

#include "mySTART.h"
#include "myLCD_HP.h"
int dem, x;
char mang[900];

int main(void)
{
	PORTC = 0x0F;
	DDRC = 0x00;
	init_LCD();
	print_LCD("READY");
	_delay_ms(1000);
	while(inbit(PINC, 0) == 1);
	_delay_ms(13);
	move_LCD(2,1);
	vietso_dec_LCD(2);
	for (dem = 0; dem < 900; dem++)
	{
		if (inbit(PINC, 0) == 1)
		{
			mang[dem] = 0;
		}
		else
			mang[dem] = 1;
		_delay_us(300);
	}
	dem = 67;
	while(1)
	{	
		move_LCD(2,1);
		print_LCD("dem = ");
		vietso_dec_LCD(dem);
		space_LCD(3);
		move_LCD(3,1);
		for (x=dem; x < dem+128; x ++)
		{
			if (mang[x] < mang[x-1])
				lcd_write(1, 0xFF);
			if (mang[x])
			{
				if (mang[x] != mang[x-1])
					lcd_write(1, 0xFF);
				else
					lcd_write(1, 0x80);
			}
			else
				lcd_write(1, 0x01);
		}
		if (inbit(PINC, 1) == 0)
			if (dem < 700)
			{
				dem++;
				_delay_ms(50);
			}
		if (inbit(PINC, 2) == 0)
			if (dem != 0)
			{
				dem--;
				_delay_ms(50);
			}
	}
}