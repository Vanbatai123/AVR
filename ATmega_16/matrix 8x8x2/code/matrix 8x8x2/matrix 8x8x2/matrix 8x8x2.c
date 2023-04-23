/*
 * matrix_8x8x2.c
 *
 * Created: 10/26/2015 5:36:18 PM
 *  Author: Administrator
 */ 
#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
int n=0;
int ma[]={
	0X81,0XEF,0XEF,0XEF,0XEF,0XEF,0XED,0XF3
	,0x03	  ,0x01		,0xC8	  ,0xCC		,0xC8	  ,0x01		,0x03	  ,0xFF		//A
	,0X00     ,0X00		,0X24	  ,0X24		,0X00	  ,0X00		,0X99	  ,0XFF		//B
	,0X81	  ,0X00		,0X3C	  ,0X3C		,0X3C	  ,0X18		,0X99	  ,0XFF		//C
	,0X00     ,0X00     ,0X3C     ,0X3C     ,0X18     ,0X00     ,0X81     ,0XFF     //D
	,0X00     ,0X00     ,0X24     ,0X24     ,0X24     ,0X3C     ,0X3C     ,0XFF     //E
	,0X00     ,0X00     ,0XE4     ,0XE4     ,0XE4     ,0XFC     ,0XFF     ,0XFF     //F
	,0X81     ,0X00     ,0X3C     ,0X3C     ,0X2C     ,0X08     ,0X89     ,0XFF     //G
	,0X00     ,0X00     ,0XE7     ,0XE7     ,0XE7     ,0X00     ,0X00     ,0XFF     //H
	,0X3C     ,0X3C     ,0X00     ,0X00     ,0X3C     ,0X3C     ,0XFF     ,0XFF     //I
	,0XFF     ,0X9F     ,0X1C     ,0X3C     ,0X00     ,0X80     ,0XFC     ,0XFF     //J
	,0X00     ,0X00     ,0XE7     ,0XC3     ,0X99     ,0X3C     ,0X7E     ,0XFF     //K
	,0XFF     ,0X00     ,0X00     ,0X3F     ,0X3F     ,0X3F     ,0X3F     ,0XFF     //L
	,0X00     ,0X00     ,0XF9     ,0XF3     ,0XF3     ,0XF9     ,0X00     ,0X00     //M
	,0X00     ,0X00     ,0XF1     ,0XE3     ,0XC7     ,0X8F     ,0X00     ,0X00     //N
	,0X81     ,0X00     ,0X3C     ,0X3C     ,0X3C     ,0X00     ,0X81     ,0XFF     //O
	,0XFF     ,0X00     ,0X00     ,0XCC     ,0XCC     ,0XC0     ,0XE1     ,0XFF     //P
	,0X81     ,0X00     ,0X3C     ,0X2C     ,0X1C     ,0X80     ,0X41     ,0XFF     //Q
	,0X00     ,0X00     ,0XCC     ,0X8C     ,0X20     ,0X71     ,0XFF     ,0XFF     //R
	,0XFF     ,0X99     ,0X30     ,0X64     ,0X0C     ,0X99     ,0XFF     ,0XFF     //S
	,0XFF     ,0XFC     ,0XFC     ,0X00     ,0X00     ,0XFC     ,0XFC     ,0XFF     //T
	,0X80     ,0X00     ,0X3F     ,0X3F     ,0X3F     ,0X00     ,0X80     ,0XFF     //U
	,0XC0     ,0X80     ,0X1F     ,0X3F     ,0X1F     ,0X80     ,0XC0     ,0XFF     //V
	,0XC0     ,0X80     ,0X3F     ,0X0F     ,0X0F     ,0X3F     ,0X80     ,0XC0     //W
	,0X3C     ,0X99     ,0XC3     ,0XE7     ,0XC3     ,0X99     ,0X3C     ,0XFF     //X
	,0XF8     ,0XF1     ,0XE3     ,0X07     ,0X07     ,0XE3     ,0XF1     ,0XF8     //Y
	,0X3C     ,0X1C     ,0X0C     ,0X24     ,0X30     ,0X38     ,0X3C     ,0XFF     //Z
	,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
	
};

int main(void)
{
	//int a=0;
	DDRA=DDRD=DDRB=0XFF;
	DDRC=0XFF;
	while(1)
	{			
		//int x=n+8;

			for (int g=0;g<8;g++)
			{
				for(int q=0;q<8;q++)
				{
					int j=0xfe;
					for (int i=0;i<8;i++)
					{
						PORTC=j;
						PORTA=~ma[i];
						_delay_ms(1);
						j=(j<<1)+1;
						
					}
					ma[g]=(ma[g]<<1)+1;
				}
			}
	}
}