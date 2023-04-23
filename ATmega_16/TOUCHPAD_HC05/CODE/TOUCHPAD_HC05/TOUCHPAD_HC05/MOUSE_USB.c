/*
 * TOUCHPAD_HC05.c
 *
 * Created: 2/10/2017 5:45:38 PM
 *  Author: Van_BasTai
 */ 
#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myMOUSE.h"
#include "F:\AVR\Header\myUART.h"

int button_status, dx, dy, count_xy, count;

int main(void)
{
	init_UART();
	Mouse_Init();
	sei();
//  clrb(UCSRB, RXCIE);
	while(1)
	{
		wdt_reset();
		usbPoll();
		if(usbInterruptIsReady())
		{
			reportBuffer.buttonMask = button_status;
			reportBuffer.dx = dx;
			reportBuffer.dy = dy;
			usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
		}
	}
}
ISR(USART_RXC_vect)
{
	data = UDR;
	if ((data > 123) && (data < 128))
		button_status = data;
	else
	{
		count_xy ++;
		if (count_xy%2==1)
			dx = data;
		else
			dy = data;
	}
	
}