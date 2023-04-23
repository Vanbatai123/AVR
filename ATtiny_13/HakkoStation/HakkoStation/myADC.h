int read_ADC(int adc_channel)//chuong trinh con doc ADC theo tung channel
{
	for (int x = 0;x < 50;x++)
	{
		ADMUX=adc_channel;
		ADCSRA|=(1<<ADSC); 					//bat dau chuyen doi
		while(inbit(ADCSRA,ADIF)==0); //cho den khi nao bit ADIF==1
	}
	return ADC;
}
void init_ADC()
{
	setb(ADMUX,REFS0);
	ADCSRA|=(1<<ADEN);
}