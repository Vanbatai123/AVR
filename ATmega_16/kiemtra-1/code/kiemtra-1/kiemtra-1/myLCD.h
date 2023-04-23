#include <avr/io.h>
#include <util/delay.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define data PORTD
#define EN 1
#define RS 0
#define LCD8BIT 0X38
#define LCD4BIT 0X28
#define EN 1
#define RS 0
int *a;int c=LCD8BIT;
void lenh(char malenh)
{
	if(*a==0x38)
	{
		setb(PORTC,EN);
		clrb(PORTC,RS);
		data=malenh;
		clrb(PORTC,EN);
		setb(PORTC,EN);
		_delay_us(100);
	}
	else
	{
		int xxx;
		xxx=data&0x0f;
		setb(PORTC,EN);
		clrb(PORTC,RS);
		data=((malenh&0xf0)|xxx);
		clrb(PORTC,EN);
		setb(PORTC,EN);
		data=(((malenh<<4)&0xf0)|xxx);
		clrb(PORTC,EN);
		setb(PORTC,EN);
		_delay_us(100);
	}
}
void dulieu(char  dulieu)//******************************__GHI 8 BIT LCD__*************************
{
	if (*a==0x38)//8bit
	{
		setb(PORTC,EN);
		setb(PORTC,RS);
		data=dulieu;
		clrb(PORTC,EN);
		setb(PORTC,EN);
		_delay_ms(2000);
	}
	else					//4bit
	{
		int xxx;
		xxx=data&0x0f;
		setb(PORTC,EN);
		setb(PORTC,RS);
		data=((dulieu&0xf0)|xxx);
		clrb(PORTC,EN);
		setb(PORTC,EN);
		data=(((dulieu<<4)&0xf0)|xxx);
		clrb(PORTC,EN);
		setb(PORTC,EN);
		_delay_ms(2000);
	}
}
void wait_LCD()//*****************************************__CHO LCD RANH__*************************
{
	_delay_ms(1000);
}
void init_LCD()//*****************************************__KHOI DONG LCD__************************
{	
	setb(PORTC,EN);
	clrb(PORTC,RS);
	data=c;
	clrb(PORTC,EN);
	setb(PORTC,EN);
	_delay_us(100);//FONT, SO LUONG LINE, PHUONG THUC GIAO TIEP		---FUNCTION SET
	a=&c;
	lenh(0x0E);//HIEN THI VA CON TRO							---DISPLAY ON/OFF
	lenh(0x07);// CACH THEM KI TU								---ENTRY MODE SET
	
	
}
void clr_LCD() //********************************************__xoa LCD__***************************
{
	lenh(0x01);
}
void home_LCD()//***************************************__DUA CON TRO VE DAU__*********************
{
	lenh(0x02);
}
void move_LCD(int dong, int cot)//********__DI CHUYEN CON TRO DEN VI TRI MONG MUON (DONG, COT)__***
{
	if (*a==0x38)
	{
		if (cot>0&&cot<=30)
		{
			if (dong==1)
			lenh(0x80-1+cot);
			else if (dong==2)
			lenh(0xC0-1+cot);
		}
	}
	else
	{
		if (cot>0&&cot<=30)
		{
			if (dong==1)
			lenh(0x80-1+cot);
			else if (dong==2)
			lenh(0xC0-1+cot);
		}
	}
}
void putChar_LCD(int kitu)//**************************__GHI 1 KI TU LEN LCD__**********************
{
	dulieu(kitu);
}
void print_LCD(char* str, unsigned char dodai)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; i<dodai; i++)
	if(str[i] > 0) putChar_LCD(str[i]);
	else putChar_LCD(' ');

}