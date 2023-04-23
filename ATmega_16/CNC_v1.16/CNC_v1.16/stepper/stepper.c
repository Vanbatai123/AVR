/*
* stepper.c
*
* Created: 10/26/2018 9:09:27 PM
*  Author: PC
*/
#include "../include.h"
#include "stepper.h"

// configure stepper pin
void stepper_config()
{
	DDR_STEPPER |= (1<<PIN_DIR_X) | (1<<PIN_DIR_Y) | (1<<PIN_STEP_X) | (1<<PIN_STEP_Y);
}

// control a stepper
void stepper_ctrl(int8_t delta_x, int8_t delta_y)
{
	// configure direction of steppers
	if (delta_x < 0) setb(PORT_STEPPER, PIN_DIR_X);
	else clrb(PORT_STEPPER, PIN_DIR_X);
	
	if (delta_y < 0) setb(PORT_STEPPER, PIN_DIR_Y);
	else clrb(PORT_STEPPER, PIN_DIR_Y);
	
	if (delta_x != 0) setb(PORT_STEPPER, PIN_STEP_X);
	else clrb(PORT_STEPPER, PIN_STEP_X);
	if (delta_y != 0) setb(PORT_STEPPER, PIN_STEP_Y);
	else clrb(PORT_STEPPER, PIN_STEP_Y);
	
	_delay_us(100);
	clrb(PORT_STEPPER, PIN_STEP_X);
	clrb(PORT_STEPPER, PIN_STEP_Y);
	_delay_us(100);
}
// run 2 stepper with direction value
void stepper_run(uint8_t dir)
{
	switch (dir)
	{
		case DIR_UP:// y + 1, x
		/* Your code here */
		break;
		
		case DIR_UP_RIGHT:
		/* Your code here */
		break;
		
		case DIR_RIGHT:
		/* Your code here */
		break;
		
		case DIR_DOWN_RIGHT:
		/* Your code here */
		break;
		
		case DIR_DOWN:
		/* Your code here */
		break;
		
		case DIR_DOWN_LEFT:
		/* Your code here */
		break;
		
		case DIR_LEFT:
		/* Your code here */
		break;
		
		case DIR_UP_LEFT:
		/* Your code here */
		break;
		
		case DIR_STAND:
		/* Your code here */
		break;
		
		default:
		/* Your code here */
		break;
	}
}
