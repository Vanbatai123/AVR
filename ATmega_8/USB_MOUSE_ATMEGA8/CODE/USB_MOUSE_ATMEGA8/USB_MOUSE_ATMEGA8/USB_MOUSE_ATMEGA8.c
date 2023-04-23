/*
 * USB_MOUSE_ATMEGA8.c
 *
 * Created: 2/10/2017 7:18:36 PM
 *  Author: Van_BasTai
 */ 

#include "F:\AVR\Header\myMOUSE.h"


int main(void)
{
	Mouse_Init();
	PORTB = 0xFF;
	DDRB = 0xff;
	while(1)
	{
		char delta = 20;
		DBG1(0x02, 0, 0);   /* debug output: main loop iterates */
		wdt_reset();
		usbPoll();
		if(usbInterruptIsReady())
		{
			if(inbit(PINB,0) == 0)
			reportBuffer.dx = delta;
			else if(inbit(PINB,1) == 0)
			reportBuffer.dx = -delta;
			else if(inbit(PINB,2) == 0)
			reportBuffer.dy = delta;
			else if(inbit(PINB,3) == 0)
			reportBuffer.dy = -delta;
			else if(inbit(PINB,4) == 0)
				reportBuffer.dWheel = 1;
			else if(inbit(PINB,5) == 0)
				reportBuffer.dWheel = -1;
			else
				reportBuffer.buttonMask = reportBuffer.dx = reportBuffer.dWheel = reportBuffer.dy = 0;
			DBG1(0x03, 0, 0);   /* debug output: interrupt report prepared */
			usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
		}
	}
}