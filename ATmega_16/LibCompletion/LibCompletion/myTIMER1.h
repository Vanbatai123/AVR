void hesochia(int div)
{
	if (div==1)
		TCCR1B=(1<<CS10);
	else if (div==8)
		TCCR1B=(1<<CS11);
	else if (div==64)
		TCCR1B=(1<<CS10)|(1<<CS11);
	else if (div==256)
		TCCR1B=(1<<CS12);
	else
		TCCR1B=(1<<CS10)|(1<<CS12);
}