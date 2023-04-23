/*int check_start()
{
	if(inbit(DATA_PIN,DATA)==0)						 // Neu co canh xuong cua tin hieu
	{
		_delay_us(500);										 // Delay de chong nhieu
		if(inbit(DATA_PIN,DATA)==0)					 // Neu dung la co canh xuong cua tin hieu
		{
			for(int i=0;i<45;i++)								 // Chia nho xung START thanh 40 doan, moi doan dai 200us
			{
				_delay_us(200);
				if(inbit(DATA_PIN,DATA)==1) 
				return 0;									 // Neu tin hieu len muc cao (1) --> khong phai xung START
			}
			while(inbit(DATA_PIN,DATA)==1);
			return 1;										 // Dung la xung start
		}
		
	}
}
//  Ham doc ma cua cac kenh
unsigned long int infr_sensor()
{
	unsigned char i;
	unsigned int Data_Infr;
	Data_Infr=0;
	if(check_start())                  // Neu dung xung START
	{
		while(inbit(DATA_PIN,DATA)==1);          // Cho het bit 1 tiep theo
		while(inbit(DATA_PIN,DATA)==0);          // Cho het bit 0 tiep theo
		for(i=0;i<24;i++)             // Lay 24 bit du lieu (bao gom 8 bit 0 va 8 bit 1 dau tien sau xung start)
		{
			//while(inbit(DATA_PORT,DATA_PIN)==0);
			_delay_us(750);           // Delay 750us de kiem tra bit
			if(inbit(DATA_PIN,DATA)==0) Data_Infr=Data_Infr*2 + 1;
			if(inbit(DATA_PIN,DATA)==1) Data_Infr=Data_Infr*2;
			while(inbit(DATA_PIN,DATA)==1);     // cho canh len
			while(inbit(DATA_PIN,DATA)==0);     // cho canh len
		}
		_delay_ms(600);                // Cho het tin hieu (chong nhieu)
	}
	return Data_Infr;                  // Gia tri ma kenh tra ve
}*/
void NgatNgoai(void) interrupt 0
{
	char IR[16]; // biến chứa bit remote
	int i=0;
	char x=0;
	P1_0=1;

	// xén phần mã remote giống nhau
	while(P3_2==0)
	{

	}
	while(P3_2==1)
	{
		
	}
	while(1)
	{
		
		if(P3_2==0)
		{
			while(P3_2 ==0)
			{;}
			i++;
		}
		
		if(i==17)
		break;
	}

	// đọc phần mã khác nhau 16 bit
	while(x<16)
	{
		
		if(P3_2==1)
		{
			tick=0;
			while(tick<7) { ; } // chờ 700 us

			if(P3_2==1) // nếu sau 700us mà tín hiệu còn thì nó là bit 1 (đỉnh bằng)
			{
				
				IR[x]=1;
				while(P3_2==1)
				{
					;
				}
			}
			else // ngược lại là đỉnh nhọn
			{
				IR[x]=0;
			}
			
			x++;
		}
		else
		{
			while(P3_2==0)
			{
				;
			}
		}
		
	}

	Delay(100); // delay 100ms cho kết thúc phần tín hiệu còn lại nếu không khi thoát hàm ngắt  nó sẽ xảy ra ngắt tiếp
}