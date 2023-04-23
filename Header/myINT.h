/************************************************************************/
/*	FileName: myInterrupt.h												*/
/*	Created: 10/6/2017 9:12:57 AM										*/
/*  Author: Van_BasTai													*/
/* 																		*/
/*	Description: File Header chua ngat ngoai 0 va 1						*/
/*																		*/
/************************************************************************/

#define INT_PORT	PORTD
#define INT_DDR		DDRD

#define INT_0		2
#define INT_1		3

void INT_init()
{
	INT_PORT |= (1<<INT_0)|(1<<INT_1);
	INT_DDR |= (0<<INT_0)|(0<<INT_1);
	
	MCUCR|=(1<<ISC01)|(1<<ISC11);// Falling edge;
	GICR|=(1<<INT0)|(1<<INT1);// Enable INT0, INT1;
}
