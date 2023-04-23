void Init_Timer_0(int div)
{
	TCNT0 = 0;
	if (div==1024)
		TCCR0|=(1<<CS00)|(1<<CS02);
	else if (div==256)
		TCCR0|=(1<<CS02);
	else if (div==64)
		TCCR0|=(1<<CS00)|(1<<CS01);
	else if (div==8)
		TCCR0|=(1<<CS01);
	else
		TCCR0|=(1<<CS00);
}
//	TIMSK|= 1<<TOIE0;// Bat ngat timer0.