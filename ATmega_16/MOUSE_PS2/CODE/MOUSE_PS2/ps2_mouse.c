/* 				
/			PS2_MOUSE.C
/
/		 PS/2 Mouse driver
/
*/

#include "E:\AVR\MOUSE_PS2\CODE\MOUSE_PS2\ps2_mouse.h"

volatile static uint8_t buffcnt; //index to the buffer
volatile static PS2_STATUS status; 
volatile static int8_t cur_x,cur_y;  //cursor cordinates as signed values
volatile static uint8_t buttons; // mouse buttons state

static uint8_t stream_mode;	//boolean value whether stream mode or not 


void MouseInit(void)
{

	stream_mode = FALSE;
	status = INIT;
	
	//low level initialisation
	PS2_Init();
	
	//enable Global interrupt
	sei();
	
	//wait until we receive self-test result and mouse id
	while( status != BAT_PASSED);
	while( status != ID_OK);

	status = INIT;	
	
	//set res = 1 count/mm
	PS2_SendToDevice(0xE8);
	while(status != ACK) ;

	status = INIT;
	PS2_SendToDevice(0x01);
	while(status != ACK) ;

	status = INIT;
	//set device in stream mode
	PS2_SendToDevice(PS2_REPORT_EN);
	
	//wait for acknowledgement(0xFA) from device
	while(status != ACK) ;
	
	//set mouse in stream mode
	stream_mode = TRUE;
	buffcnt = 0;
	
	return;
}

void Decode(uint8_t byteIn)
{
	if(stream_mode == TRUE) {
	
		switch(buffcnt) {
		
			case 0	:	buttons = byteIn; break;
				
			case 1	:	cur_x += byteIn;
						if(cur_x > MAX_CUR_X) cur_x = MAX_CUR_X;
						else if(cur_x < MIN_CUR_X) cur_x = MIN_CUR_X;
						break;
						
			case 2	:	cur_y += byteIn;
						if(cur_y > MAX_CUR_Y) cur_y = MAX_CUR_Y;
						else if(cur_y < MIN_CUR_Y) cur_y = MIN_CUR_Y;
						break;
		}
				
		if(++buffcnt == BUFF_SIZE) buffcnt = 0;

	}
	else {
	
		switch(byteIn) {
		
			case PS2_ACK			: 	status = ACK; break;
		
			case PS2_ERROR			:	status = ERROR; break;
			
			case PS2_MOUSE_ID		:	status = ID_OK; break;

			case PS2_BAT_SUCCESS	:	status = BAT_PASSED; break;
		
			default 				:	break;
		
		}
		
	}
	
	return;
}


//return current mouse X cord 
uint8_t MouseGetX(void)
{
	return (uint8_t)cur_x;
}


//return current mouse Y cord
uint8_t MouseGetY(void)
{
	return (uint8_t)cur_y;
}


//return the first data packet which contain button states
uint8_t MouseGetButtons(void)
{	
	return buttons&(0x07); //only last 3 bits
}

