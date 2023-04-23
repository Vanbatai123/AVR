/*
 * LED.h
 *
 * Created: 1/29/2013 1:38:04 PM
 *  Author: dungdothe
 */ 


#ifndef LED_H_
#define LED_H_

#include "StdTypes.h"

#define LED1_ON           LED1_PORT |= (1 << LED1_PIN)
#define LED1_OFF          LED1_PORT &= ~(1 << LED1_PIN)
#define LED1_TOGGLE       (LED1_PORT ^= (1 << LED1_PIN))

#define LED2_ON           LED2_PORT |= (1 << LED2_PIN)
#define LED2_OFF          LED2_PORT &= ~(1 << LED2_PIN)
#define LED2_TOGGLE       (LED2_PORT ^= (1 << LED2_PIN))

void LED_Configuration(void);


#endif /* LED_H_ */
