/*
* HakkoStation.c
*
* Created: 10/29/2017 4:50:57 PM
*  Author: Van_BasTai
*/


#include "mySTART.h"
#include "myADC.h"

#define CONTROL		4
#define ADJUST		3
#define SENSOR		1
#define DATA		1
#define CLK			0

#define HeaterOn()	clrb(PORTB, CONTROL)
#define HeaterOff()	setb(PORTB, CONTROL)

//char codeseg[] = {3,2,1,0};
unsigned char code[] = {0xD7, 0x14, 0xCD, 0x5D, 0x1E, 0x5B, 0xDB, 0x15, 0xDF, 0x5F};
unsigned char up = 0,down = 0;
void xuatled(char led, char data)
{
	for(char i = 0;i < 18;i++)//18bit data
	{
		clrb(PORTB, CLK);
		if(i < 4)
		{
			if(i == 4 - led)
			clrb(PORTB, DATA);
			else
			setb(PORTB, DATA);
		}
		else if (i > 9)
		{
			if((data<<(i-10))&0x80)
			setb(PORTB, DATA);
			else
			clrb(PORTB, DATA);
		}
		setb(PORTB, CLK);
	}
	//latch data
	clrb(PORTB, DATA);
	setb(PORTB, DATA);
	//send data to output
	clrb(PORTB, CLK);
	clrb(PORTB, DATA);
	setb(PORTB, DATA);
}

void vietso(int num, int t)
{
	for (int x = 0;x < t;x++)
	{
		xuatled(1, code[(num/100)%10]);
		xuatled(2, code[(num/10)%10] | up);
		xuatled(3, code[num%10] | down);
		xuatled(4, 0x0F);
	}
}

int main(void)
{
	
	DDRB |= (1 << DATA)|(1 << CLK)|(1 << CONTROL);
	HeaterOff();
	init_ADC();
	unsigned int t_AdjNew, t_AdjOld=0, t_Sensor=0;
	while(read_ADC(ADJUST) < 50)
	{
		xuatled(1, 0x9E);
		xuatled(2, 0xCB);
		xuatled(3, 0x96);
		xuatled(4, 0xD7);
	}
	for (unsigned int x = 0;x < 5000;x++)
	{
		xuatled(1, 0x9E);
		xuatled(2, 0xCB);
		xuatled(3, 0x96);
		xuatled(4, 0xD7);
	}
	while(1)
	{
		t_AdjNew = read_ADC(ADJUST) / 2 + 2;
		while (read_ADC(ADJUST) < 1)
		{
			//HeaterOff();
			//xuatled(1, 0x00);
			xuatled(2, 0xD7);
			xuatled(3, 0x8B);
			xuatled(4, 0x8B);
		}
		if (t_AdjNew < t_AdjOld - 2 || t_AdjNew > t_AdjOld + 2)
		{
			vietso(t_AdjNew, 600);
			t_AdjOld = t_AdjNew;
		}
		else
		{
			vietso(t_Sensor, 1000);
		}
		t_Sensor = read_ADC(SENSOR);
		t_Sensor=( (5*2200L*t_Sensor) / (11253 - t_Sensor))-245;
		if (t_Sensor < t_AdjOld)
		{
			HeaterOn();
			up = 0x20;
			down=0x00;
		}
		else if (t_Sensor > t_AdjOld + 30)
		{
			HeaterOff();
			down = 0x20;
			up = 0x00;
		}
	}
}