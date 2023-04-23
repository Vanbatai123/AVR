//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// Author : VAN BA TAI															     ;
// Email:   VANBATAI1357@gmail.com													 ;
// Date :   09/02/2016	                                                             ;
// Version: 1.0                                                                      ;
// Title:   myDS1307                                                                 ;
// Description: Thu vien cua Ds1307							                         ;
//                                                                                   ;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define read 1
#define write 0
int i=0;
int data_received[7],write_data;
int	Second=57, Minute=32, Hour=20, Day=1, Date=9, Month=2, Year=16, Mode=0, AP=1;
//Mode: chon che do 12h hoac 24h, Mode nam o bit 6 cua thanh ghi HOURS,
//Mode=1: 12H, Mode=0: 24H
//AP: bien chi AM hoac PM trong Mode 12h, AP nam o bit 5 cua thang ghi HOURS,
//AP=1:PM, AP=0: Am
int tData[7],reg[]={0x00}, Time_count=0; //dinh nghia cac bien tam
//------------master------------------------
void init_DS1307(void) // Function to initialize master(KHOI TAO CHUC NANG MASTER)
{
	TWBR=0x01;    // Bit rate(TOC DO)
	TWSR=(0<<TWPS1)|(0<<TWPS0);    // Setting prescalar bits
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
}

void TWI_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI(XOA CO NGAT TWI,DAT DIEU KIEN BAT DAU SDA,Enable TWI)
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted(CHO DEN KHI DIEU KIEN BAT DAU DUOC TRUYEN)
	//while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgement(KIEM TRA ACK)
}
void TWI_write_address(unsigned char data)
{
	TWDR=data;    // Address and write instruction(DATA=ADDRESS+WRITE)
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted(CHO TOI KHI 1 BYTE TRONG TWDR DUOC TRUYEN THANH CONG  )
	//while((TWSR & 0xF8)!= 0x18);  // Check for the acknoledgement   (CHECK ACK)
}

void TWI_read_address(unsigned char data)
{
	TWDR=data;    // Address and read instruction(DATA=ADDRESS+READ)
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received(CHO TOI KHI 1 BYTE DUOC NHAN THANH CONG)
	move_LCD(6,7);
	vietso_dec_LCD(2);
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknoledgement
	move_LCD(6,8);
	vietso_dec_LCD(3);
}

void TWI_write_data(int data[])
{
	TWDR=data[i];    // put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	//while((TWSR & 0xF8) != 0x28); // Check for the acknoledgement
}

void TWI_read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
	data_received[0]=TWDR;
	
}

void TWI_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	//while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}

void goidulieu(int thanhghi,int data[])
{	
	
	TWI_start();
	TWI_write_address((0x68<<1)+write);
	TWDR=thanhghi;    // put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	for (int j=0;j<7;j++)
	{
		TWDR=data[j];    // put data in TWDR
		TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	}
	TWI_stop();
	_delay_ms(1);
	
}
void doc()
{
	for (int j=0;j<7;j++)
	{
		TWI_start();
		TWI_read_address((0x68<<1)+read);
		TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
		while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
		tData[j]=TWDR;
		TWI_stop();
		_delay_ms(1);
	}
}
void ghi(int reg[])
{
	TWI_start();
	TWI_write_address((0x68<<1)+write);
	TWI_write_data(reg);
	TWI_stop();
	_delay_ms(1);
}
int BCD2Dec(int BCD)
{
	int L, H;
	L=BCD & 0x0F;
	H=(BCD>>4)*10;
	return (H+L);
}
int Dec2BCD(int Dec)
{
	int L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}
void Display(void)//chuong trinh con  hien thi thoi gian doc tu DS1307
{
	Second 	= BCD2Dec(tData[0] & 0x7F);
	Minute 	= BCD2Dec(tData[1] & 0x7F);
	
	if (Mode !=0) 	Hour = BCD2Dec(tData[2] & 0x1F); //mode 12h
	else 		  	Hour = BCD2Dec(tData[2] & 0x3F); //mode 24h
	
	Date   	= BCD2Dec(tData[4]);
	Month	= BCD2Dec(tData[5]);
	Year	= BCD2Dec(tData[6]);
	
	//clr_LCD();		//xoa LCD
	//in gio:phut:giay
	move_LCD(1,1);
	print_LCD("Time: ");
	move_LCD(1,7);
	if ((Hour/10)==0)
	{
		print_LCD("0");
	}
	vietso_dec_LCD(Hour);
	move_LCD(1,9);
	putchar_LCD('-');
	move_LCD(1,10);
	if ((Minute/10)==0)
	{
		print_LCD("0");
	}
	vietso_dec_LCD(Minute);
	move_LCD(1,12);
	putchar_LCD('-');
	move_LCD(1,13);
	if ((Second/10)==0)
	{
		print_LCD("0");
	}
	vietso_dec_LCD(Second);
	if (Mode !=0)
	{										//mode 12h
		move_LCD(1,16);
		if (inbit(tData[2],5))
		putchar_LCD('P'); //kiem tra bit AP, if AP=1
		else putchar_LCD('A');
	}
	//in nam-thang-ngay
	move_LCD(2,1);
	print_LCD("Date: ");
	move_LCD(2,13);
	if ((Year/10)==0)
	{
		print_LCD("0");
	}
	vietso_dec_LCD(Year);
	move_LCD(2,9);
	putchar_LCD('-'); //in Nam
	move_LCD(2,10);
	if ((Month/10)==0)
	{
		print_LCD("0");
	}
	vietso_dec_LCD(Month);
	move_LCD(2,12);
	putchar_LCD('-'); //in thang
	move_LCD(2,7);
	if ((Date/10)==0)
	{
		print_LCD("0");
	}
	vietso_dec_LCD(Date);	//in Ngay
	move_LCD(2,17);
}
void chuanbi()
{
	tData[0]=Dec2BCD(Second);
	tData[1]=Dec2BCD(Minute);
	if (Mode!=0) tData[2]=Dec2BCD(Hour)|(Mode<<6)|(AP<<5); //mode 12h
	else tData[2]=Dec2BCD(Hour);
	tData[3]=Dec2BCD(Day);
	tData[4]=Dec2BCD(Date);
	tData[5]=Dec2BCD(Month);
	tData[6]=Dec2BCD(Year);
}