/*
 * _4SERVO.c
 *
 * Created: 10/14/2016 4:33:14 PM
 *  Author: Van BasTai
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__// mo khoa ham delay ???????????????
#include <util/delay.h>
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#define SERVO_DDR DDRA//Dinh nghia ddr va port cua servo
#define SERVO_PORT PORTA
//ham dieu khien 2 servo
void servo_2(int servo1,float start1,int stop1,int servo2,float start2,int stop2)
{
	start1=940+5.17*start1;
	start2=940+5.17*start2;
	stop1=940+5.17*stop1;
	stop2=940+5.17*stop2;
	int temp,temp1,temp2;
	if (start1>stop1)//neu = 0 thi goc tang len.
		temp1=1;
	else
		temp1=0;
	if (start2>stop2)
		temp2=1;
	else 
		temp2=0;
	while((start1<stop1)||(start2<stop2))
	{
		
		if (temp1==0)
			start1+=1;
		else
			start1-=1;
		if (temp2==0)
			start2+=1;
		else
			start2-=1;
		if (start1<start2)//lay start1 < start2
		{
			setb(SERVO_PORT,servo1);
			setb(SERVO_PORT,servo2);
			_delay_us(start1);
			clrb(SERVO_PORT,servo1);
			_delay_us(start2-start1);
			clrb(SERVO_PORT,servo2);
		}
		else
		{
			setb(SERVO_PORT,servo1);
			setb(SERVO_PORT,servo2);
			_delay_us(start2);
			clrb(SERVO_PORT,servo1);
			_delay_us(start1-start2);
			clrb(SERVO_PORT,servo2);
		}
	}
}
int main(void)
{
	SERVO_DDR=0xFF;
	SERVO_PORT=0X00;
	while(1)
    {		
		servo_2(0,0,180,1,180,0);
	}
}