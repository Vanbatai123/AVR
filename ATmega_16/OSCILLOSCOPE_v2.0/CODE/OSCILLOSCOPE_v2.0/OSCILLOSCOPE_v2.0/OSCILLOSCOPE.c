/*
 * OSCILLOSCOPE_v2.0.c
 *
 * Created: 01/21/2017 10:45:19 AM
 * Author: Van_BasTai
 */ 

#include <avr/io.h>// Khai bao thu vien cac cong vao ra.
#define F_CPU 16000000UL// Khai bao thach anh dao dong cho vi dieu khien.
#include <util/delay.h>// Goi thu vien chua ham delay.
#include <stdio.h>// Goi thu vien chua ham sprintf dung trong file myLCD_TFT.h.
#include <avr/pgmspace.h>// Thu vien de chuyen mang sang hang so de không ton dung luong RAM.
#include <avr/interrupt.h>// Thu vien ngat cua AVR.
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)// Set 1 bit bat kì len 1.
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))// Clear 1 bit bat ki xuong 0.
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit// doc gia tri 1 bit bat ki.
//----------------------------------------------------------------------------------------------
#include "myLCD_TFT.h"// File thu vien LCD.
//----------------------------------------------------------------------------------------------
#define ADC_POSITION		4
#define ADC_VOLT			5
#define ADC_TIME			6
#define ADC_OSCIL			7
#define COLOR_OSCIL			GREEN
#define COLOR_NET			RED
#define COLOR_TEXT			YELLOW
//----------------------------------------------------------------------------------------------
int x = -1, position, count, div_time = 255;
float div_volt = 4;
int y[320];
//----------------------------------------------------------------------------------------------
int read_ADC(int adc_channel)// Chuong trinh con doc ADC theo tung kenh.
{
	ADMUX=adc_channel|0x40;// Thiet lap thanh ghi ADMUX theo kenh ADC can doc.
	ADCSRA|=(1<<ADSC);// Bat dau chuyen doi.
	while(inbit(ADCSRA,ADIF)==0); // Cho den khi bit ADIF = 1.
	return ADCW;// Tra lai gia tri ADC doc duoc cho ham read_ADC.
}
//----------------------------------------------------------------------------------------------
void init_ADC()// Khoi tao bo ADC
{
	setb(ADMUX,REFS0);
	ADCSRA=(1<<ADEN)|(1<<ADPS2);
}
//----------------------------------------------------------------------------------------------
void Set_Pos_y(int y)// Chuong trinh con hien thi toa do (x, y) moi lên LCD.
{
	TFT_SetAddrWindow(x, y, x, y+ 1);
	TFT_Flood(COLOR_OSCIL,3);
}
//----------------------------------------------------------------------------------------------
void Clear_Pos_y(int y)// Chuong trinh con xoa toa do (x, y) cu và chinh lai luoi toa do.
{
	if ((x%20 == 0) && (y != 0))
	{	
		TFT_SetAddrWindow(x, y, x, y+ 1);
		TFT_Flood(COLOR_NET,3);
	}
	else
	{
		TFT_SetAddrWindow(x, y, x, y+ 1);
		TFT_Flood(SCREEN_COLOR,3);
	}
	if ((y%20 == 0)&&(y != 0))
		TFT_DrawPixel(x, y,COLOR_NET);
	if (y%20 == 19)
		TFT_DrawPixel(x, y+1,COLOR_NET);
}
//----------------------------------------------------------------------------------------------
void Start()// Giao dien khoi dau may do xung
{
	TFT_FillScreen(SCREEN_COLOR);
	TFT_Print(0, 215, "/*", BIG, COLOR_OSCIL);
	TFT_Print(0, 190, " * DO AN VI DIEU KHIEN", BIG, COLOR_OSCIL);
	TFT_Print(0, 165, " * DE TAI: MAY DO XUNG", BIG, COLOR_OSCIL);
	TFT_Print(0, 140, " * SVTH: VAN BA TAI", BIG, COLOR_OSCIL);
	TFT_Print(0, 115, " * GVHD: DANG PHUOC VINH", BIG, COLOR_OSCIL);
	TFT_Print(0, 90, " * LOP : 13CDT1", BIG, COLOR_OSCIL);
	TFT_Print(0, 65, " * NHOM: 13.05B", BIG, COLOR_OSCIL);
	TFT_Print(0, 40, " * MSSV: 101130174", BIG, COLOR_OSCIL);
	TFT_Print(0, 15, " */", BIG, COLOR_OSCIL);
	_delay_ms(5700);
	TFT_FillScreen(SCREEN_COLOR);
	TFT_Print(65, 180, "R-OSCILLOSCOPE", BIG, COLOR_OSCIL);
	TFT_Print(75, 145, "LOADING:   %", BIG, COLOR_OSCIL);
	TFT_DrawRectangle(9, 109, 310, 127, BLUE );
	TFT_DrawRectangle(7, 107, 312, 129, BLUE );
	for (int count = 10; count < 310; count++)
	{
		TFT_Number_Dec(175, 145, (count - 10)/3 + 1, BIG, COLOR_OSCIL);
		TFT_DrawLineY(count, 110, 126, MAGENTA);
	}
	_delay_ms(2000);
	TFT_Print(75, 75, "   DONE!!!   ", BIG, COLOR_OSCIL);
	_delay_ms(2000);
	TFT_Print(20, 40, "''LET THE GAME BEGIN''", BIG, COLOR_OSCIL);
	_delay_ms(2000);
}
//----------------------------------------------------------------------------------------------
int main(void)
{
	init_ADC();// Khoi dong bo chuyen doi ADC.
	TFT_Init();// Khoi dong LCD.
	TFT_SetRotation(LEFT);// Thiet lap huong cua LCD.
	TFT_FillScreen(SCREEN_COLOR);// Thiet lap mau nen LCD.
	setb(PORTD, 7);// Công tac OFF_SWITCH, thiet lap chân cua công tac la input.
	clrb(DDRD, 7);
	_delay_ms(1000);
	if (inbit(PIND, 7) == 0)
		Start();
	TFT_FillScreen(SCREEN_COLOR);// Thiet lap mau nen LCD.
	TCCR0|=(1<<CS00)|(1<<CS01);// Thiet lap he so chia cho bo dem timer0 bang 64.
 	for (count = 0;count<16; count++)// Ve luoi toa do moi ô 20*20 pixel.
		TFT_DrawLineY(count*20, 20, 239, COLOR_NET);
	for (count = 1;count<12; count++)// Ve luoi toa do moi ô 20*20 pixel.
		TFT_DrawLineX(0, 319, count*20,  COLOR_NET);
	TFT_Print(0	 , 0, "T=  ms", BIG, COLOR_TEXT);
	TFT_Print(106, 0, "V=     v", BIG, COLOR_TEXT);
	TFT_Print(212, 0, "P=", BIG, COLOR_TEXT);
 	TIMSK|= 1<<TOIE0;// Bat ngat timer0.
	sei();// Bat ngat toan cuc.
    	while(1) // Vong lap vô han de cho ngat.
    	{
		
    	}// Ket thuc vong lap vô han while(1).
}// Ket thuc hàm main().
//----------------------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect) // Ham ngat tran timer0.
{
	x++;// Tang x len 1 don vi.
	position = read_ADC(ADC_POSITION)/127 + 1;// doc và tinh toan gia tri Position.
	if (x >= 320)// Neu x >=320 thi:
	{
		x = 0;// Tro ve diem bat dau tren truc X.
		div_time = 1.25*(4 + div_time)/5;
		div_time*=5;// tính giá tri T de hien thi len LCD.
		TFT_Number_Dec(26, 0, div_time, BIG, COLOR_TEXT);// Hien thi gia tri T.
		TFT_Number_thapphan(132, 0, 8/div_volt, BIG, COLOR_TEXT);// Hien thi gia tri V.
		TFT_Number_Dec(232, 0, position-1, BIG, COLOR_TEXT);// Hien thi gia tri P.
		while(inbit(PIND, 7) == 0);// Cong tac OFF_SWITCH.
	}// Ket thuc ham if.
	div_time = read_ADC(ADC_TIME)/200 ;// tinh div_time.
	div_volt = read_ADC(ADC_VOLT)/127 + 1;// tinh div_volt.
	Clear_Pos_y(y[x]);// Xoa diem (x, y) cu và chinh lai luoi toa do.
	y[x] = ((read_ADC(ADC_OSCIL)-512)*div_volt/20.48 + position*20);// Luu lai gia tri (x, y) moi vao mang y[].
	if (y[x] < 20)
		y[x] = 240;// Neu giá tri y nam trong vung hien thi cac gia tri P, T, V thi gan cho y 1 gia tri nam ngoai man hinh.
	Set_Pos_y(y[x]);// hien thi diem (x, y) moi len LCD.
	TCNT0 = 255 - 20*div_time;// Thiet lap lai bien dem Timer0 (TCNT0).
}// Ket thúc ngat tràn timer0.
 
