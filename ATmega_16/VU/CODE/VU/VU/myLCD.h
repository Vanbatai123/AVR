#include <avr/io.h>
#define setb(thanhghi,bit) thanhghi=(1<<bit)|thanhghi
#define clb(thanhghi,bit) thanhghi=~((1<<bit))&thanhghi
#define inbit(thanhghi,bit) (thanhghi &(1<<bit))>>bit
#include <util/delay.h>
#define RS 2
#define  EN 3


// LCD 1602 che do 4 bit
// Chan du lieu
/* 
    D4 - PORTC.4
	D5 - PORTC.5
	D6 - PORTC.6
	D7 - PORTC.7
*/
// Chan dieu khien
/*
    RS - PORTC.2
	EN  - PORTC.3
*/

void Truyenlenh (char Command)
{
		clb(PORTC,RS);
		setb(PORTC,EN);
		PORTC=((PORTC&0x0f)|(Command&0xf0));//xoa 4 bit cao cua PORTC va cong 4 bit cao cua lenh
		clb(PORTC,EN);
		setb(PORTC,EN);
		PORTC=((PORTC&0x0f)|((Command<<4)&0xf0));//xoa 4 bit cao va cong 4 bit thap cua lenh
		clb(PORTC,EN);
		_delay_ms(5);
}

void Truyendulieu (char data)
{
		setb(PORTC,RS);
		setb(PORTC,EN);
		PORTC=((PORTC&0x0f)|(data&0xf0));//xoa 4 bit cao cua PORTC va cong 4 bit cao cua du lieu
		clb(PORTC,EN);
		setb(PORTC,EN);
		PORTC=((PORTC&0x0f)|((data<<4)&0xf0));//xoa 4 bit cao va cong 4 bit thap cua du lieu
		clb(PORTC,EN);
		_delay_ms(1);
}

// Ham khoi tao LCD
void LCD_Init(void)
{
		DDRC=0xff;
		PORTC=0xfd;
		Truyenlenh(0x02); // di chuyen con tro ve dau man hinh
		Truyenlenh(0x28); // giao tiep 4 bit, hien thi 2 dong, kich thuoc font 5x7
		Truyenlenh(0x0c); // bat hien thi va tat con tro
		Truyenlenh(0x06); // tu dong di chuyen con tro den vi tri tiep theo
		Truyenlenh(0x01); // xoa toan bo lcd
}
// Ham xoa LCD
void LCD_Clear()
{   
	PORTC&=~(1<<RS);
	Truyenlenh(0x01);
	_delay_ms(1);
}
// Ham viet 1 chuoi ki tu ra LCD
void LCD_Puts (char *s)
{
		while (*s)
		{
			Truyendulieu(*s);
			s++;
		}
}

void Viet_So(int so)
{
		unsigned char nghin,tram,chuc,donvi;
		nghin=so/1000;
		tram=(so%1000)/100;
		chuc=(so%1000%100)/10;
		donvi=so%10;
		Truyendulieu(nghin+48);
		Truyendulieu(tram+48);
		Truyendulieu(chuc+48);
		Truyendulieu(donvi+48);
}

void LCD_Vitri (unsigned char line,unsigned char colum)
{
		uint8_t Ad;
		Ad=64*(line-1)+(colum-1)+0x80;
		Truyenlenh(Ad);
}

void LCD_So(unsigned char line,unsigned char colum,char so)
{
		uint8_t Ad;
		Ad=64*(line-1)+(colum-1)+0x80;
		Truyenlenh(Ad);
		Viet_So(so);
}