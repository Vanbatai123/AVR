/*
 * LCD_HP.h
 *
 * Created: 4/10/2018 8:50:48 PM
 *  Author: Van_BasTai
 */ 


#ifndef LCD_HP_H_
#define LCD_HP_H_


#define LCD_PORT		PORTA
#define LCD_DDR			DDRA

#define CS				4 //Chon chip	  ss
#define SDA				5 //Data SPI
#define AO				6 //Chot du lieu Command va Data (C/D)
#define SCK				7 //Clock SPI	   ck

void LCDHP_Write(uint8_t cd, uint8_t byte);
void LCDHP_Move(uint8_t x, uint8_t y);
void LCDHP_Clear();
void LCDHP_Putc(uint8_t dat);
void LCDHP_Puts(char *s);
void LCDHP_Config();
void LCDHP_PrintD(long num);
void LCDHP_PrintF(float num);
void LCDHP_PrintB(unsigned long num);
void LCDHP_PrintH(long num);
void LCDHP_Move1Pixel(uint8_t x, uint8_t y);
void picture(int dat);
void LCDHP_Space(uint8_t num);


#endif /* LCD_HP_H_ */