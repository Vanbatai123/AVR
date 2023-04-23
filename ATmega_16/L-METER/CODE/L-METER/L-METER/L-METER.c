/*
 * L-METER-V1.0_PROJECT
 *
 * Created: 9/22/2016 9:08:19 AM
 *  Author: Van BasTai
 */ 

#include "E:\AVR\Header\mySTART.h"
#include "E:\AVR\Header\myLCD_HP.h"
int x=0;
long n;
int main(void)
{
    DDRA=0xff;
    DDRC=0xff;
    DDRB=0xff;
    DDRD=0x00;
    PORTD=0xff;
    PORTB=0x00;
    
    init_LCD();
	setb(TCCR1B,CS10);
	//setb(TCCR1B,CS11);
	//setb(TCCR1B,CS12);
	TCNT1=0;

    //NGAT NGOAI
    setb(PORTD,0);
    MCUCR=(1<<ISC01)|(1<<ISC11);
    GICR=(1<<INT0)|(1<<INT1);
    sei();
	while(1)
    {
		
    }

}
ISR(INT0_vect)
{
	n=TCNT1;
	TCNT1=0;
}