/*
* PlasticHeater.c
*
* Created: 12/9/2018 12:26:11 PM
* Author : Van BasTai
*/

#include "include.h"
#include "uart/uart.h"


#define M1_DDR	DDRC
#define M1_PORT	PORTC
#define M1_PIN	PINC

#define M2_DDR	DDRA
#define M2_PORT	PORTA
#define M2_PIN	PINA

#define C1_DDR	DDRB
#define C1_PORT	PORTB
#define C1_PIN	PINB

#define C2_DDR	DDRD
#define C2_PORT	PORTD
#define C2_PIN	PIND

#define C1_PIN_EMER		2
#define C1_PIN_ORIENT	0
#define C1_PIN_CLAMP	1
#define C1_PIN_SWITCH	3 // linkage = 1, single = 0

#define C2_PIN_EMER		3
#define C2_PIN_ORIENT	2
#define C2_PIN_CLAMP	4
#define C2_PIN_SWITCH	5 // linkage = 1, single = 0

#define M1_PIN_Z		0 // cyan
#define M1_PIN_CHEM		1 // white
#define M1_PIN_HOLDIN	2 // green
#define M1_PIN_HOLDOUT	3 // yellow
#define M1_PIN_EP2		4 // blue
#define M1_PIN_EP1		5 // brown
#define M1_PIN_HEATER	6 // red

#define M2_PIN_CHEM		7 // white
#define M2_PIN_HOLDIN	6 // green
#define M2_PIN_HOLDOUT	5 // yellow
#define M2_PIN_EP2		4 // blue
#define M2_PIN_EP1		3 // brown
#define M2_PIN_HEATER	2 // red

#define LINKAGE			1
#define SINGLE			0

#define DELAY			1700

#define IDLE			0
#define CLAMP_0			1
#define CLAMP_1			2
#define CLAMP_2			3
#define CLAMP_3			4
#define ORIENT2			5

uint8_t _t_heater = 0, _t_hold = 0;
uint8_t HC165_read()
{
	clrb(PORTC, 7);
	setb(PORTC, 7);
	uint8_t val = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		clrb(PORTD, 7);
		val = (val << 1) | inbit(PIND, 6);
		setb(PORTD, 7);
	}
	return val;
}

void checkEmergency()
{
	while (inbit(C1_PIN, C1_PIN_EMER) == 0 || inbit(C2_PIN, C2_PIN_EMER) == 0)
	{
		UART_println("EMERGENCY!!!");
		_delay_ms(500);
	}
}

void delay()
{
	checkEmergency();
	_delay_ms(DELAY);
}

int main(void)
{
	_delay_ms(100);
	
	M2_DDR = 0xFC;
	M2_PORT = 0x00;
	M1_DDR = 0xFF;
	M1_PORT = 0x00;
	
	C1_PORT = 0x0F;
	C1_DDR = 0x00;
	C2_PORT = 0x7C | (0x03 & C2_PORT);
	C2_DDR = 0x80 | (0x03 & C2_DDR);
	
	UART_config(115200);
	UART_println("OK");
	// emergency button config
	setb(C1_PORT, C1_PIN_EMER);
	clrb(C1_DDR, C1_PIN_EMER); // interrupt 2
	
	setb(C2_PORT, C2_PIN_EMER);
	clrb(C2_DDR, C2_PIN_EMER); // interrupt 1
	
	//clrb(MCUCSR, ISC2); // Falling edge on INT2;
	//setb(MCUCR, ISC11); // Falling edge on INT1;
	//GICR |= (1 << INT1) | (1 << INT2);// Enable INT2, INT1;
	//GIFR |= (1 << INTF1) | (1 << INTF2);// clear interrupt flag INT2, INT1;
	
	uint8_t  modee = SINGLE, step = IDLE;
	uint32_t cnt = 0;
	//enable global interrupt
	//sei();
	while (1)
	{
		while (step == IDLE)
		{
			checkEmergency();
			_t_heater = HC165_read();
			_t_hold = (_t_heater & 0x0F) * 2;
			_t_heater = (_t_heater >> 4) * 2;
			if (cnt++ > 150000)
			{
				UART_print("IDLE - t_heat = ");
				UART_printNum(_t_heater, DEC);
				UART_print(",\tt_hold = ");
				UART_printNumln(_t_hold, DEC);
				cnt = 0;
			}
			// read mode
			if (inbit(C1_PIN, C1_PIN_SWITCH) == SINGLE && inbit(C2_PIN, C2_PIN_SWITCH) == SINGLE)
			{
				modee = LINKAGE;// single mode
			}
			else
			{
				modee = SINGLE;
			}
			if(inbit(C1_PIN, C1_PIN_ORIENT) == 0 || inbit(C2_PIN, C2_PIN_ORIENT) == 0)// check orient button
			{
				checkEmergency();
				_delay_ms(100);
				if(inbit(C1_PIN, C1_PIN_ORIENT) == 0 || inbit(C2_PIN, C2_PIN_ORIENT) == 0)// check orient button
				{
					step++;
					UART_print("ORIENT PRESSED: ");
					UART_printNumln(step, DEC);
					
					setb(M1_PORT, M1_PIN_Z);// Truc z chay vao = 1
					setb(M1_PORT, M1_PIN_CHEM);// Chem2= 1
					if(modee) setb(M2_PORT, M2_PIN_CHEM);// Chem1 = 1
					delay();
					
					setb(M1_PORT, M1_PIN_EP1);// Truc z chay vao = 1
					setb(M1_PORT, M1_PIN_EP2);// Truc z chay vao = 1
					if(modee) setb(M2_PORT, M2_PIN_EP1);// Chem2 = 1
					if(modee) setb(M2_PORT, M2_PIN_EP2);// Chem2 = 1
					delay();
				}
			}
		}
		
		checkEmergency();
		if(inbit(C1_PIN, C1_PIN_CLAMP) == 0 || inbit(C2_PIN, C2_PIN_CLAMP) == 0)// check clamp button
		{
			checkEmergency();
			_delay_ms(100);
			if(inbit(C1_PIN, C1_PIN_CLAMP) == 0 || inbit(C2_PIN, C2_PIN_CLAMP) == 0)// check clamp button
			{
				if (++step > CLAMP_3) step = CLAMP_0;
				UART_print("CLAMP PRESSED: ");
				UART_printNumln(step, 10);
				switch (step)
				{
					case CLAMP_0:
					clrb(M1_PORT, M1_PIN_HOLDIN);// ep_in1 = 1
					if(modee) clrb(M2_PORT, M2_PIN_HOLDIN);// ep_in2 = 1
					clrb(M1_PORT, M1_PIN_HOLDOUT);// ep_in1 = 1
					if(modee) clrb(M2_PORT, M2_PIN_HOLDOUT);// ep_in2 = 1
					delay();
					break;
					
					case CLAMP_1:
					setb(M1_PORT, M1_PIN_HOLDOUT);// ep_out1 = 1
					if(modee) setb(M2_PORT, M2_PIN_HOLDOUT);// ep_out2 = 1
					delay();
					break;
					
					case CLAMP_2:
					setb(M1_PORT, M1_PIN_HOLDIN);// ep_out1 = 1
					if(modee) setb(M2_PORT, M2_PIN_HOLDIN);// ep_out2 = 1
					delay();
					break;
					
					case CLAMP_3:
					clrb(M1_PORT, M1_PIN_HOLDOUT);// ep_out1 = 1
					if(modee) clrb(M2_PORT, M2_PIN_HOLDOUT);// ep_out2 = 1
					delay();
					break;
					default:
					/* Your code here */
					break;
				}
			}
		}
		if((inbit(C1_PIN, C1_PIN_ORIENT) == 0 || inbit(C2_PIN, C2_PIN_ORIENT) == 0) && step == CLAMP_2)// check orient button
		{
			checkEmergency();
			_delay_ms(100);
			if((inbit(C1_PIN, C1_PIN_ORIENT) == 0 || inbit(C2_PIN, C2_PIN_ORIENT) == 0) && step == CLAMP_2)// check orient button again
			{
				// ko ep
				clrb(M1_PORT, M1_PIN_EP1);
				clrb(M1_PORT, M1_PIN_EP2);
				if(modee) clrb(M2_PORT, M2_PIN_EP1);
				if(modee) clrb(M2_PORT, M2_PIN_EP2);
				delay();
				
				//rut chem
				clrb(M1_PORT, M1_PIN_CHEM);
				if(modee) clrb(M2_PORT, M2_PIN_CHEM);
				delay();
				
				// ha ban nhiet
				setb(M1_PORT, M1_PIN_HEATER);
				if(modee) setb(M2_PORT, M2_PIN_HEATER);
				delay();
				
				// kep gia nhiet
				setb(M1_PORT, M1_PIN_EP1);
				setb(M1_PORT, M1_PIN_EP2);
				if(modee) setb(M2_PORT, M2_PIN_EP1);
				if(modee) setb(M2_PORT, M2_PIN_EP2);
				
				//cho gia nhiet
				for (uint8_t i = 0; i < _t_heater; i++)
				{
					checkEmergency();
					_delay_ms(1000);
				}
				
				// rut kep
				clrb(M1_PORT, M1_PIN_EP1);
				if(modee) clrb(M2_PORT, M2_PIN_EP1);
				clrb(M1_PORT, M1_PIN_EP2);
				if(modee) clrb(M2_PORT, M2_PIN_EP2);
				delay();
				
				// rut ban gia nhiet
				clrb(M1_PORT, M1_PIN_HEATER);
				if(modee) clrb(M2_PORT, M2_PIN_HEATER);
				delay();
				
				// kep gia nhiet
				setb(M1_PORT, M1_PIN_EP1);
				if(modee) setb(M2_PORT, M2_PIN_EP1);
				setb(M1_PORT, M1_PIN_EP2);
				if(modee) setb(M2_PORT, M2_PIN_EP2);
				delay();
				
				//cho dinh cung
				for (uint8_t i = 0; i < _t_hold; i++)
				{
					checkEmergency();
					_delay_ms(1000);
				}
				
				//rut clamp
				clrb(M1_PORT, M1_PIN_HOLDOUT);// ep_out1 = 1
				if(modee) clrb(M2_PORT, M2_PIN_HOLDOUT);// ep_out2 = 1
				clrb(M1_PORT, M1_PIN_HOLDIN);// ep_out1 = 1
				if(modee) clrb(M2_PORT, M2_PIN_HOLDIN);// ep_out2 = 1
				delay();
				
				// rut ep
				clrb(M1_PORT, M1_PIN_EP1);
				if(modee) clrb(M2_PORT, M2_PIN_EP1);
				clrb(M1_PORT, M1_PIN_EP2);
				if(modee) clrb(M2_PORT, M2_PIN_EP2);
				
				clrb(M1_PORT, M1_PIN_Z);
				delay();
				step = IDLE;
			}
		}
	}
}