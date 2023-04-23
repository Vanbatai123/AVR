/*
* InOut.c
*
* Created: 07/25/2018 10:25:53 PM
*  Author: Van_BasTai
*/


#include "include.h"



#define LED		5
#define BUTTON	4


int main(void)
{
	while(1)
	{
		if (analogRead(0) > 300)
		{
			digitalWrite(LED, HIGH);
		}
		else
		{
			digitalWrite(LED, 0);
		}
	}
}