/*
 * LCD_HOMEPHONE.c
 *
 * Created: 2/10/2016 9:53:17 PM
 *  Author: Administrator
 */ 
//Khoi dong SPI o che do Master
/****************************************************
****Lap trinh giao tiep Graphic LCD 128x64 SPI (Gphone)****
*******Code by Vu Van Thai - SangTaoClub.Net*********
- Mot so lenh hien thi cho GLCD:
	1. lcd_write(0,0xC8); //Hien thi nguoc, 0xC0 - Binh thuong
	2. lcd_write(0,0xA7); //Hien thi am ban, 0xA6 - Binh thuong
	3. lcd_write(0,0xA5); //Hien thi all pixel, 0xA4 - Binh thuong
- Cac ham giao tiep GLCD:
	1. lcd_init(); //Khoi tao GLCD
	2. lcd_write(CD, DATA_BYTE); 
		//CD Select: 0 - Command, 1 - Display Data
		//DATA_BYTE: Du lieu can ghi vao LCD
	3. clr_LCD(); //Xoa man hinh
	4. move_LCD(X, Y);
		//X la vi tri hien thi 0-20 (Text font 5x7), Graphic 0-127
		//Y la dong hien thi 0-7
	5. lcd_putchar('Data_ASCII'); //Hien thi mot ky tu ASCII len man hinh
	6. lcd_puts("String"); //Hien thi mot chuoi len man hinh
****************************************************/

#include "include.h"


int main()				//Chuong trinh chinh
{
	int dem=0;
	LCDHP_Config();
	_delay_ms(100);
	LCDHP_Move(2,7);
	LCDHP_Puts("VAN BA TAI");
	_delay_ms(1000);
	LCDHP_Move(5,8);
	LCDHP_Puts("13CDT1");
	//move_LCD(3,0);
	//print_LCD("SangTaoClub.Net");
	LCDHP_Move(8,7);								  
	LCDHP_Puts("Num: ");
	_delay_ms(2000);
	while(1)
	{
		LCDHP_Move(8,2);
		LCDHP_PrintD(dem++);
		_delay_ms(90);
	}
}


