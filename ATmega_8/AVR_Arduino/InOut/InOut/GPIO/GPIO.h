/*
 * GPIO.h
 *
 * Created: 7/25/2018 2:47:17 PM
 *  Author: taivb
 */ 


#ifndef GPIO_H_
#define GPIO_H_

// pin mode definition

#define INPUT			0x0
#define OUTPUT			0x1
#define INPUT_PULLUP	0x2

#define LOW				0
#define HIGH			1

#define A0				14
#define A1				15
#define A2				16
#define A3				17
#define A4				18
#define A5				19

extern uint8_t pinToBit(uint8_t pin);
extern volatile uint8_t *pinToDDR(uint8_t pin);
extern volatile uint8_t *pinToPort(uint8_t pin);
extern volatile uint8_t *pinToInput(uint8_t pin);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
uint8_t digitalRead(uint8_t pin);





#endif /* GPIO_H_ */