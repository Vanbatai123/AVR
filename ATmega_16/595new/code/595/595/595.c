/*
 * 595.c
 *
 * Created: 8/2/2016 10:17:52 AM
 *  Author: Administrator
 */
#include <avr/io.h>
#define F_CPU 12000000UL
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
char ma[]={0X3F,0x06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
#define PORT PORTD
#define DDR DDRD
//#define clk clrb(PORTC,1)
//#define latch clrb(PORTC,2)
#include <avr/io.h>
void ship(int data)
{
	for (int i=8;i>0;i--)
	{
		if((data&0x80)==0x80)
			setb(PORT,0);
		else
			clrb(PORT,0);
		data=data<<1;
		clrb(PORT,1);//xung clk cua thanh ghi dich
		setb(PORT,1);
	}
	//clrb(PORT,2);
	//setb(PORT,2);
	_delay_ms(1);
}
int main(void)
{
	PORT=0X00;
	DDR=0X07;
	DDRA=0xff;
    while(1)
    {
		
			ship(0xf0);//_delay_ms(10);
			PORTA=0x12;//_delay_ms(1000000);
			//a=a>>1;
    }
}