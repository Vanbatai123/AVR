/*
 * LED.c
 *
 * Created: 1/29/2013 1:37:50 PM
 *  Author: dungdothe
 */ 
#include "LED.h"

void LED_Configuration(void)
{
  /* LED_RED */
  LED1_DDR |= (1 << LED1_PIN);
  /* LED_GREEN */
  LED2_DDR |= (1 << LED2_PIN); 
}
