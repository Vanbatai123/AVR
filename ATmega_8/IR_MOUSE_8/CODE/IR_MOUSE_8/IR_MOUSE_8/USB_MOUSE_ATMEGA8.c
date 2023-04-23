/*
 * IR_MOUSE_8.c
 *
 * Created: 2/10/2017 7:18:36 PM
 *  Author: Van_BasTai
 */ 

#define  F_CPU 16000000UL
#include "myMOUSE.h"
#include "myTIMER_1.h"

#define UP			0xA956
#define DOWN		0x59A6
#define RIGHT		0x9966
#define LEFT		0xD926
#define R_CLICK		0x21DE
#define L_CLICK		0x7986
#define WHELL_DOWN	0x9F6
#define WHELL_UP	0x39C6

unsigned int IR_code, timer;
char count, IR_Check, left, right, up, down;

int main(void)
{
	Mouse_Init();
	Init_Timer_1(8);
	MCUCR|=(0<<ISC01)|(1<<ISC11);
	GICR|=(1<<INT1);
	sei();
	while(1)
	{                /* main event loop */
		DBG1(0x02, 0, 0);   /* debug output: main loop iterates */
		wdt_reset();
		usbPoll();
		if(usbInterruptIsReady() && IR_Check)
		{
			DBG1(0x03, 0, 0);   /* debug output: interrupt report prepared */
			usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
			IR_Check = 0;
			reportBuffer.buttonMask = reportBuffer.dx = reportBuffer.dWheel = reportBuffer.dy = 0;
		}
		if(usbInterruptIsReady() && reportBuffer.buttonMask ==0)
		{
			DBG1(0x03, 0, 0);   /* debug output: interrupt report prepared */
			usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
		}
	}
}
//Ham ngat doc ma IR
ISR(INT1_vect)
{
	count++;
	timer = TCNT1/200;
	TCNT1 = 0;
	if ((timer < 140)&&(timer > 130))
		right = left = up = down = 0;
	else if ((timer > 310))
	{
		count = 0;
		IR_Check = 1;
		if(IR_code == RIGHT)
		{
			if (right < 20)
				right+= 3;
			reportBuffer.dx = right;
		}
		else if(IR_code == LEFT)
		{
			if (left > -15)
				left-= 3;
			reportBuffer.dx = left;
		}
		else if(IR_code == UP)
		{
			if (up > -15)
				up-= 3;
			reportBuffer.dy = up;
		}
		else if(IR_code == DOWN)
		{
			if (down < 20)
				down+= 3;
			reportBuffer.dy = down;
		}
		else if(IR_code == R_CLICK)
			reportBuffer.buttonMask = 2;
			
		else if(IR_code == L_CLICK)
			reportBuffer.buttonMask = 1;
			
		else if(IR_code == WHELL_DOWN)
			reportBuffer.dWheel = -5;
		else if(IR_code == WHELL_UP)
			reportBuffer.dWheel = 5;
	}
	if(count > 18)
	{
		if ((timer > 20)&&(timer < 30))
		IR_code = (IR_code<<1) + 1;
		else if(timer < 15)
		IR_code = IR_code<<1;
	}
}