#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit//DDUNG TOI DBIT
int i=0;
int main(void)
{
	DDRA=0xFF;//cho phep xuat
	PORTB=0xFF;
	char ma[]={0X3F,0x06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
	while(1)
	{
		PORTA=ma[i];
		if (inbit(PINB,2)==0)
		{
			i++;
			if (i==10)
			i=0;
			PORTA=ma[i];
			_delay_ms(300);
		}
		if (inbit(PINB,1)==0)
		{
			i--;
			if (i==-1)
			i=9;
			PORTA=ma[i];
			_delay_ms(300);
		}
	}
}