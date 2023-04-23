/*
 * lenxuongquetcot.c
 *
 * Created: 11/3/2015 11:07:03 AMxxxxxxxxxxxxxxxxxxx
 *  Author: Administrator
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
int a,j,k,i;
char hango[16]={
			 	 0x81,0x00,0x7e,0x7e,0x00,0x81,0xff,0xff
				,0xFF,0xC9,0xB6,0xB6,0xB6,0xC9,0xFF,0xFF
				};
char xuat[8];
int main(void)
{
    DDRA=0xff;
    DDRC=0xff;
    PORTA=0xff;
	while(1)
	{
		for(int f=0;f<8;f++)
			xuat[f]=hango[f];
		for(i=0;i<=8;i++)
		
		{
			for(k=0;k<=2;k++)
			{
				a=0x01;
				for(j=0;j<=7;j++)
				{
					
					PORTA=a;
					PORTC=((xuat[j])>>(8-i))|(0xff<<i);
					_delay_ms(5);
					a=a*2;
				}

				
			}
		}
			 for(k=0;k<3;k++)
			 {
				 for (int y=0;y<3;y++)
				 {
				 
					 a=0x01;
					 for(j=0;j<=7;j++)
					 {
						 
						 PORTA=a;
						 PORTC=(xuat[j]<<(k+1))+(0xff>>(7-k));
						 _delay_ms(5);
						 a=a<<1;
						 
					} 
				 }
			}
			
		/*for(i=0;i<=8;i++)
		
		{
			for(k=0;k<=2;k++)
			{
				a=0x01;
				for(j=0;j<=7;j++)
				{
					
					PORTA=a;
					PORTC=((hango[j+8])>>(8-i))|(xuat[j]<<i);
					_delay_ms(5);
					a=a*2;
				}

				
			}
		}*/
		
	}
}