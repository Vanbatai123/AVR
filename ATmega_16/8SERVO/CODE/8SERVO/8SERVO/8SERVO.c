/*
 * _8SERVO.c
 *
 * Created: 10/9/2016 8:31:38 PM
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
int ZZZ=1200,YYY=1780;
//ham dieu khien servo
void servo_control(int stt,float gocbatdau,float gocketthuc,int domin)
{
	SERVO_DDR=0xFF;
	if ((gocbatdau<181)&&(gocketthuc<181))
	{
		gocbatdau=500+11*gocbatdau;//tinh thoi gian xung = 1
		gocketthuc=500+11*gocketthuc;
		
		if (gocbatdau>gocketthuc)
		{
			for (int x=gocbatdau;x>gocketthuc;)
			{
				setb(PORTA,stt);
				_delay_us(x);
				clrb(PORTA,stt);
				_delay_us(20000-x);
				x=x-domin;
			}
		}
		else
		{
			for (int x=gocbatdau;x<gocketthuc;)
			{
				setb(PORTA,stt);
				_delay_us(x);
				clrb(PORTA,stt);
				_delay_us(20000-x);
				x=x+domin;
			}
		}
	}
}
void dung4chan()
{
	for (int x=0;x<20;x++)//4 servo cap 1
	{
		ZZZ=45;YYY=180-ZZZ;
		ZZZ=500+11*ZZZ;
		YYY=500+11*YYY;
		PORTA=0x0a;
		_delay_us(ZZZ);
		PORTA=0;
		_delay_us(20000-ZZZ);
		PORTA=0xa0;
		_delay_us(YYY);
		PORTA=0;
		_delay_us(20000-YYY);
	}
	for (int x=0;x<100;x++)//4 servo cap 2
	{
		ZZZ=35;YYY=180-ZZZ;
		ZZZ=500+11*ZZZ;
		YYY=500+11*YYY;
		PORTA=0x05;
		_delay_us(ZZZ);
		PORTA=0;
		_delay_us(20000-ZZZ);
		PORTA=0x50;
		_delay_us(YYY);
		PORTA=0;
		_delay_us(20000-YYY);
	}
}
void servo_2(int servo1,float goc1,int servo2,float goc2)
{
		goc1=11*goc1+500;
		goc2=11*goc2+500;
		setb(SERVO_PORT,servo1);
		setb(SERVO_PORT,servo2);
		_delay_us(goc1);
		clrb(SERVO_PORT,servo1);
		_delay_us(goc2-goc1);
		clrb(SERVO_PORT,servo2);
		_delay_us(20000-goc2);
}
void servo_4(int servo1,float goc1,int servo2,float goc2,int servo3,float goc3,int servo4,float goc4)
{
	goc1=11*goc1+500;
	goc2=11*goc2+500;
	goc3=11*goc1+500;
	goc4=11*goc2+500;
	setb(SERVO_PORT,servo1);
	setb(SERVO_PORT,servo2);
	setb(SERVO_PORT,servo3);
	setb(SERVO_PORT,servo4);
	_delay_us(goc1);
	clrb(SERVO_PORT,servo1);
	_delay_us(goc2-goc1);
	clrb(SERVO_PORT,servo2);
	_delay_us(goc3-goc2);
	clrb(SERVO_PORT,servo3);
	_delay_us(goc4-goc3);
	clrb(SERVO_PORT,servo4);
	_delay_us(20000-goc4);
}
int main(void)
{
	//le cap 1 chan cap 2
	SERVO_DDR=0xFF;
	DDRB=0X00;
	PORTB=0XFF;
	while(1)
    {
		//
		
		if(inbit(PINB,0)==0)
		{
			for(int x=0;x<15;x++)
			{
				servo_2(1,45,3,45);
				servo_2(5,135,7,135);
			}
		}
		//	dung4chan();
			
		/*if(inbit(PINB,0)==0)
		{
			servo_control(1,45,90,50);
			_delay_ms(1000);
			servo_control(1,90,0,50);
		}	*/
		
		if(inbit(PINB,1)==0)
		{
			//chan 3
			servo_control(4,143,90,50);//nhat chan len
			
			servo_control(5,135,45,50);//quay lui
			
			servo_control(4,90,143,50);//ha chan
			
		}
		
		
		if(inbit(PINB,3)==0)
		{
			
			//chan 1
			servo_control(0,37,90,50);//nhat chan len
			
			servo_control(1,90,0,50);//quay lui
			
			servo_control(0,90,37,50);//ha chan
			//step 2 chan 3
			servo_control(2,37,90,50);//nhat chan len
			
			servo_control(3,135,45,50);//quay lui
			
			servo_control(2,90,37,50);//ha chan
			//step 3 chan 1
			/*servo_control(0,37,90,50);//nhat chan len
			
			servo_control(1,90,0,50);//quay lui
			
			servo_control(0,90,37,50);//ha chan
			
			
			servo_control(1,0,45,40);
			servo_control(3,45,90,40);
			servo_control(5,90,135,40);
			servo_control(7,135,90,40);
			*/
			for(int x=0;x<15;x++)
			{
				servo_2(1,45,3,90);
				servo_2(5,45,7,90);
			}
		}	
		
		if(inbit(PINB,2)==0)
		{
			//step 2 chan 4
			servo_control(6,143,90,50);//nhat chan len
			
			servo_control(7,135,180,50);//quay lui
			
			servo_control(6,90,143,50);//ha chan
			//step 3 chan 3
			servo_control(4,143,90,50);//nhat chan len
			
			servo_control(5,45,135,50);//quay lui
			
			servo_control(4,90,143,50);//ha chan
			//step 4 chan 2
			servo_control(2,37,90,50);//nhat chan len
			
			servo_control(3,45,90,50);//quay lui
			
			servo_control(2,90,37,50);//ha chan
			/*
			servo_control(1,45,90,40);
			servo_control(3,90,135,40);
			servo_control(5,135,90,40);
			servo_control(7,180,135,40);*/
			for(int x=0;x<15;x++)
			{
				servo_2(1,90,3,125);
				servo_2(5,90,7,135);
			}
		}
	}
}