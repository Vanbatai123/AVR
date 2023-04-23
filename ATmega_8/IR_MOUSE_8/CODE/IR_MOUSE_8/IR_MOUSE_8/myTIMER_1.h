void Init_Timer_1(int div)
{
	TCNT1=0;
	if (div==1024)
		TCCR1B=(1<<CS10)|(1<<CS12);
	else if (div==256)
		TCCR1B=(1<<CS12);
	else if (div==64)
		TCCR1B=(1<<CS10)|(1<<CS11);
	else if (div==8)
		TCCR1B=(1<<CS11);
	else
		TCCR1B=(1<<CS10);
}