/*
* UART.c
*
* Created: 10/03/2017 8:01:57 PM
*  Author: Van_BasTai
*/


#include "mySTART.h"
#include "myLCD_HP.h"
#include "myUART.h"

#define HANG	0
#define COT		1


int dataa, count = 0;
int vitri[2];

int main(void)
{
	init_UART();
	init_LCD();
	sei();
	int a=0;
	DDRC = 0x00;
	PORTC = 0xFF;
	while(1)
	{
		move_LCD(1,1);
		print_UART("vanbatai13031995\n");
		_delay_ms(1000);
		// 		int inbyte = getchar_UART();
		// 		move_LCD(1, 1);
		// 		vietso_dec_LCD(inbyte);
		// 		print_LCD("     ");
	}
}
ISR(USART_RXC_vect)
{
	vitri[count] = UDR;
	count++;
	if (count == 2)
	{
		count = 0;
		move_LCD(1,1);
		print_LCD("Vi tri xe:");
		move_LCD(2,1);
		print_LCD("Hang:");
		vietso_dec_LCD(vitri[HANG]);
		space_LCD(3);
		move_LCD(3,1);
		print_LCD("Cot:");
		vietso_dec_LCD(vitri[COT]);
		space_LCD(3);
	}
}