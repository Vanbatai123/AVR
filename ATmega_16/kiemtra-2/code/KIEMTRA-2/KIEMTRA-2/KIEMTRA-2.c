#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit//DDUNG TOI DBIT
char ma[]={0X3F,0x06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
void quetled(int so)
{
	int n,t,c,dv;
	
	n=so/1000;
	t=(so%1000)/100;
	c=(so%100)/10;
	dv=so%10;
	
	
	
	setb(PORTD,0);
	PORTA=~ma[dv];
	_delay_us(600);
	PORTA=0xff;
	clrb(PORTD,0);
	setb(PORTD,1);
	PORTA=~ma[c];
	_delay_us(600);
	PORTA=0xff;
	clrb(PORTD,1);
	setb(PORTD,2);
	PORTA=~ma[t];
	_delay_us(600);
	PORTA=0xff;
	clrb(PORTD,2);
	setb(PORTD,3);
	PORTA=~ma[n];
	_delay_us(600);
	PORTA=0xff;
	clrb(PORTD,3);
}
int main(void)
{
	DDRD=0x0F;
	DDRA=0xFF;//cho phep xuat
	PORTD=0xFF;
	int a=0;
	while(1)
	{
		//cau b
		for (int i=0;i<1;i++)
		{
			quetled(a);
		}
		a++;
		if (a%100==60)
		{
			a=a+40;
		}
		if (a==1100)
		{
			a=0;
		}
		//cau a
		/*for (int i=0;i<2;i++)
		{
			quetled(a);
		}
		if (inbit(PIND,6)==0)
		{
			
			for (;a<101;a++)
			{
			
			for (int i=0;i<2;i++)
			{
				quetled(a);
			}
			}
		}
		if (inbit(PIND,7)==0)
		{
			
			for (;a>0;a--)
			{
				
				for (int i=0;i<2;i++)
				{
					quetled(a);
				}
			}
		}*/
	}
}