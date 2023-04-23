#include <avr/io.h>
# define F_CPU 8000000UL
#include <util/delay.h>

#define inbit(thanhghi,sobit) (thanhghi&(1<<sobit))>>sobit
#define setb(thanhghi,sobit) thanhghi=thanhghi|(1<<sobit)
#define clb(thanhghi,sobit) thanhghi=thanhghi&(~(1<<sobit))
#define DATA 1
#define SCK 0
#define	RCK 2
void truyen595 (char a)
{
	char i,b;
	for(i=0;i<=7;i++)
	{
		b=a&0x80;
		if(b==0)
		clb(PORTC,DATA);
		else
		setb(PORTC,DATA);
		a=a<<1;
		clb(PORTC,SCK);
		setb(PORTC,SCK);
	}
	clb(PORTC,RCK);
	setb(PORTC,RCK);
}
int main(void)
{DDRC=0xff;
	truyen595(0x30);
	while(1)
	{
		truyen595(7);
	}
}