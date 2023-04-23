unsigned int read_ADC(int adc_channel)//chuong trinh con doc ADC theo tung channel	
{
	ADMUX=adc_channel|0x40;
	ADCSRA|=(1<<ADSC); 					//bat dau chuyen doi
	while(inbit(ADCSRA,ADIF)==0); //cho den khi nao bit ADIF==1
	return ADCW;
}
void init_ADC()
{
	setb(ADMUX,REFS0);
	ADCSRA=(1<<ADEN)|(1<<ADPS2);
}