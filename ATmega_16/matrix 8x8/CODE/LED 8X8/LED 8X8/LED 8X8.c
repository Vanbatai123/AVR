#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
int ma[]={
			// 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
			// 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
			 0x03	  ,0x01		,0xC8	  ,0xCC		,0xC8	  ,0x01		,0x03	  ,0xFF		//A
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
		};
void quet()
{
	
	int xuat[208];
	
	
	//int i=0;
		int u=0;int j=0;
	for (int h=0;h<25;h++)
	{
		int c=0x80;
		int x=u+8;
		for (;u<x;u++)
		{
			xuat[u]=ma[u];
		}
		
		for (int q=0;q<8;q++)
		{
			for (int g=0;g<10;g++)
			{
				int b=0;
				
				for (int i=u-8;i<x;i++)
				{
					PORTA=(1<<(b));
					PORTB=xuat[i];
					_delay_ms(1);
					b++;
				
				}
			}
		
			//a=a>>1;
			
			for (j=u-8;j<x;j++)
			{
				xuat[j]=(ma[j]>>(q+1))+(c&(ma[j+8]<<(7-q)));
			}
			c=(c>>1)+0x80;
		}
	}
}
int main(void)
{
	//int a=0;
	DDRA=0XFF;
	DDRB=0XFF;
    while(1)
    {
		PORTA=0x40;
		PORTB=0xfb;
	   //quet();
    }
}