/* 				
/			PS2_LOW.C
/
/		Low level PS2 protocol driver
/	
/		Author: Visakhan C
/		June 3, 2012
/
*/

#include "E:\AVR\MOUSE_PS2\CODE\MOUSE_PS2\ps2_low.h"


static volatile uint8_t bitcount,bufferOut,edge;

extern void Decode(uint8_t);



//wait until clock becomes low
static void WaitClockLow(void)
{
	while(PIN_CLOCK & (1<<CLOCK_PIN)) 
		;
}

//wait until clock becomes high
static void WaitClockHigh(void)
{
	while( !(PIN_CLOCK & (1<<CLOCK_PIN)) ) 
		;
}

//Wait until Data becomes low
static void WaitDataLow(void)
{
	while(PIN_DATA & (1<<DATA_PIN)) 
		;
}

//wait until Data becomes high
static void WaitDataHigh(void)
{
	while( !(PIN_DATA & (1<<DATA_PIN)) ) 
		;
}
	
void PS2_SendToDevice(uint8_t byteOut)
{	
	
	uint8_t bitCount=0,oddParity=1;
	
	//disable INT2
	GICR &= ~(1<<INT2);
	
	PORT_CLOCK |= (1<<CLOCK_PIN); //start with both lines high  
	PORT_DATA |= (1<<DATA_PIN);
	DDR_DATA |= (1<<DATA_PIN);  
	DDR_CLOCK |= (1<<CLOCK_PIN); //clock and DATA as output
	
	_delay_us(2);
	PORT_CLOCK &= ~(1<<CLOCK_PIN); //pull CLK low
	
	_delay_us(250);  //a delay >100us
	
	PORT_DATA &= ~(1<<DATA_PIN); //now pull DATA low and
	_delay_us(2);
	DDR_CLOCK &= ~(1<<CLOCK_PIN);  //release clock
	

	while(bitCount < 8)
	{
		WaitClockLow();
		
		//set/reset DATA
		if(byteOut&(1<<bitCount)) { 
			PORT_DATA |= (1<<DATA_PIN);
			oddParity ^= 0x01;
		}	
		else
			PORT_DATA &= ~(1<<DATA_PIN);
			
		WaitClockHigh();
		
		bitCount++;
	}
	
	WaitClockLow();
	
	//send odd parity
	if(oddParity)
		PORT_DATA |= (1<<DATA_PIN);
	else
		PORT_DATA &= ~(1<<DATA_PIN);
	
	WaitClockHigh();
	
	//release DATA to send stop bit
	DDR_DATA &= ~(1<<DATA_PIN);
	
	WaitClockLow(); 
	WaitClockHigh();
	
	WaitDataLow(); //ACK bit from device
	
	WaitClockLow(); //11th clock
	WaitClockHigh();
	
	WaitDataHigh(); //all bits sent
	
	_delay_us(1);

	//enable INT2
	GIFR = (1<<INTF2);
	GICR |= (1<<INT2);
	
}

//low level initialisation
void PS2_Init(void)
{
	MCUCSR = (0<<ISC2);   // set INT2 interrupt on falling edge
	bitcount = 11;		//count down the bits
	edge = 0;			
	
	DDR_CLOCK &= ~(1<<DATA_PIN); //start with both lines
	DDR_DATA &= ~(1<<CLOCK_PIN); //as input
	
	GICR |= (1<<INT2); //enable INT2 interrupt
	
}	

ISR(INT2_vect)
{
	static uint8_t byteIn;
	
	//interruted on falling edge, so read DATA
	if(!edge)  {
	
		if(bitcount>2 && bitcount<11) {
			
			byteIn = (byteIn>>1);
			if(PIN_DATA & (1<<DATA_PIN) )
				byteIn |= 0x80;
		}
		
		MCUCSR = (1<<ISC2); //set interrupt on rising edge
		edge = 1;
	}
	else {	//interrupted on rising edge
	
		MCUCSR = (0<<ISC2); //set interrupt on falling edge
		edge = 0;
	
		if(--bitcount == 0) { //all bits received?
		
			Decode(byteIn); //now decode the byte received 
			bitcount = 11; //reset bit count
		}
	}
	
}