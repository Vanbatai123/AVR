int data, dem;

void putchar_UART(int kitu)
{
	while (inbit(UCSRA,UDRE) == 0); //cho den khi bit UDRE=1 Neu = 1 thi viet code vao
	UDR=kitu;
}
void print_UART(char* str)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; str[i]!=0; i++) 
		putchar_UART(str[i]);
}
void println_UART(char* str)//******__HIEN THI CHUOI KI TU__*********************
{
	int i;
	for (i=0; str[i]!=0; i++) 
		putchar_UART(str[i]);
	putchar_UART('\r');
}
void vietso_dec_UART(long num)
{
	char dis[10];
	sprintf(dis, "%li", num);
	print_UART(dis);
}
void vietso_bin_UART(int num)
{
	int mask=0X80;
	//hien thi byte trang thai duong ma nhi phan
	for (int i=0;i<8;i++)
	{
		if ((mask&num))
		vietso_dec_UART(1);
		else
		vietso_dec_UART(0);
		mask>>=1;
	}
}
void vietso_hex_UART(long num)
{
	char dis[10];
	sprintf(dis, "0x%lx", num);
	print_UART(dis);
	
}
void vietsothapphan_UART(float num)
{
	int x;
	x=(num*1000)/1000;
	vietso_dec_UART(x);
	putchar_UART('.');
	x=(int)(num*1000);
	x=x%1000;
	vietso_dec_UART(x);
}
uint8_t getchar_UART( void )
{
	while ((UCSRA & (1 << RXC)) == 0);
	// Do nothing until data have been received and is ready to be read from UDR
	return UDR; // return the byte
}
void init_UART()
{
	//UART
	UBRRH=0;		//set toc do truyen baud
	UBRRL=51;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
}
void space_UART(char num)
{
	for (;num>0;num--)
		putchar_UART(32);
}
void newline_UART()
{
	putchar_UART(13);
}
 /*ISR(USART_RXC_vect)
 {
	 data=UDR;
	 putChar_UART(data);
	 _delay_ms(50);
 }*/