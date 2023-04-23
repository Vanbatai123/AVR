/*
 * BIENTAN.c
 *
 * Created: 12/10/2016 9:31:37 AM
 *  Author: Administrator
 */ 


#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myTIMER_1.h"
long speed;
int div=1024;

int main(void)
{
	_delay_ms(1000);
	DDRA=DDRB=0xFF;
	init_LCD();
	hesochia(div);
	
	//NGAT NGOAI
	setb(PORTD,2);
	MCUCR=(1<<ISC01)|(1<<ISC11);
	GICR=(1<<INT0)|(1<<INT1);
	sei();
	_delay_ms(100);
	while(1)
	{
		move_LCD(1,1);
		print_LCD("  |*----*****----*|");
		move_LCD(2,1);
		print_LCD("DH BACH KHOA DA NANG");
		move_LCD(4,1);
		print_LCD(" DA DIEN TU UNG DUNG");
		move_LCD(6,1);
		print_LCD("   BIEN TAN DK DC");
		int tocdo = F_CPU*60/div/speed;
		if ((tocdo<1600)&&(tocdo>100))
		{
			move_LCD(8,1);
			print_LCD("TOC DO : ");
			vietso_LCD(tocdo);
			print_LCD("   ");
			move_LCD(8,17);
			print_LCD("(RPM)");
			PORTB=220+F_CPU/div/speed;
			
		}
		_delay_ms(500);
	}

}
ISR(INT1_vect)
{
	speed=TCNT1;
	TCNT1=0;
}