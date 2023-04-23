#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
int n=0;

int main(void)
{
	DDRB=0x00;
	PORTB=0xff;
	DDRD=0xff;
	PORTD=0x00;
	//PORTD=0x0f;
	//DDRA=0x01;
	//setb(TCCR1A,COM1A1);//CHON DANG TIN HIEU RA
	//setb(TCCR1A,COM1B1);
	//CHON CHE DO
	//setb(TCCR1A,WGM11);
	//setb(TCCR1B,WGM12);
	//setb(TCCR1B,WGM13);
	TCCR1A=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B=(1<<WGM12)|(1<<WGM13)|(1<<CS10);
	
	
	//setb(TCCR1B,CS10);//CHON HE SO CHIA
	//setb(TCCR1B,CS11);
	//OCR1B=10000;
	ICR1=7100; //20micro second
	//TCNT1=65530;
	setb(TIMSK,TOIE1);
	sei();//clrb(PORTD,0);
	while(1)
	{
		
		OCR1B=ICR1/2;
		if (inbit(PINB,0)==0)
		{			
			OCR1B=15000;_delay_ms(3000);
		}
		if (inbit(PINB,1)==0)
		{
			OCR1B=10000;_delay_ms(3000);
		}
	}
}