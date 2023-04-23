‪#‎include‬ <16F887.h>
‪#‎device‬‬ ADC=16‬
‪#‎FUSES‬‬ NOWDT //No Watch Dog Timer
#FUSES NOBROWNOUT //No brownout reset
#FUSES NOLVP //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
‪#‎use‬‬ delay(crystal=20000000)
#use rs232(UART1,baud=9600,parity=N,bits=8,stream=UART1,xmit=PIN_B7,rcv=PIN_B6)
‪#‎define‬‬ LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_DATA4 PIN_D3
#define LCD_DATA5 PIN_D4
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D6
#include <lcd.c>
int32 ma;
int i=0;
int1 mode,done=0,temp[21];
int32 time;
‪#‎INT_EXT‬‬
VOID EXT_ISR()
{
	if(mode==0)
	{
		set_timer1(0);
		ext_INT_edge (L_TO_H);
		mode=1;
	}
	else if(mode==1)
	{
		time=get_timer1();
		if((i==1)&&((time>1800)||(time<1500)))
			i=0;
		if((time>750)&&(time<800)&&(i!=1))
			temp[i]=1;
		else if((time>340)&&(time<410))
			temp[i]=0;
		i++;
		if(i==21)
		{
			done=1;
			i=0;
		}
		mode=0;
		ext_INT_edge (H_TO_L);
	}
}
void main()
{
	setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
	enable_interrupts (INT_EXT);
	ext_INT_edge (H_TO_L); // Ch?n ng?t theo sý?n âm
	enable_interrupts (GLOBAL);
	lcd_init();
	temp[1]=1;
	while(TRUE)
	{
		if(done==1)
		{
			ma=0;
			for(int j=0;j<=20;j++)
			{
				ma<<=1;
				if(temp[j]==1)
				ma=ma|0x01;
			}
			lcd_gotoxy(1,1);
			printf(lcd_putc,"\f%ld ",ma);
			fprintf(UART1,"\n%ld ",ma);
			done=0;
		}
	}
}

