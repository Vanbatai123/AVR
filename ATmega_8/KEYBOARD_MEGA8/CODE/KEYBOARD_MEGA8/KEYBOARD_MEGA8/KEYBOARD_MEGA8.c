/* Name: main.c
 * Project: HID-Test
 * Author: Christian Starkjohann
 * Creation Date: 2006-02-02
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id$
 */
#include "F:\AVR\Header\myKEYBOARD.h"

/* ------------------------------------------------------------------------- */

int	main(void)
{
	PORTB = 0xff;   /* activate all pull-ups */
	DDRB = 0;       /* all pins input */
	PORTC = 0xff;   /* activate all pull-ups */
	DDRC = 0;       /* all pins input */
	KEYBOARD_Init();
	int i;
	int code[] ={61};
	while (1)	
	{
		wdt_reset();
		usbPoll();
		if (inbit(PINB,1)==0)
			send_enable = 1;
		if (send_enable == 1)
			i=0;
		
		if(send_enable && usbInterruptIsReady())
		{
			if (code[i] == 61)
				reportBuffer[0] = MOD_ALT_LEFT;
			else
				reportBuffer[0] = 0;
			if (i < 1)	
				reportBuffer[1] = code[i];
			else
				reportBuffer[1] = 0;
			_delay_ms(1000); 
			usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
			i++;
			if (i == 1)
				send_enable = 0;
			

		}
	}
}
