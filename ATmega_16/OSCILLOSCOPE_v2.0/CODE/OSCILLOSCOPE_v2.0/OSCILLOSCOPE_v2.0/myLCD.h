#define sbi(sfr,bit) sfr|=_BV(bit)
#define cbi(sfr,bit) sfr&=~(_BV(bit))

#define EN 3
#define RS 2

#define CTRL PORTB
#define DDR_CTRL DDRB

#define DATA_O PORTB
#define DATA_I PINB
#define DDR_DA DDRB
//------------------------------------------------------------------------------
char Read2Nib()
{
	char HNib, LNib;
	DATA_O|=0xF0;

	sbi(CTRL,EN); //enable
	DDR_DA &=0x0F; //set 4 bits cao cua PORT DATA lam input
	HNib=DATA_I & 0xF0;
	cbi(CTRL,EN); //disable

	sbi(CTRL,EN); //enable
	LNib = DATA_I & 0xF0;
	cbi(CTRL,EN); //disable
	LNib>>=4;
	return (HNib|LNib);
}
//------------------------------------------------------------------------------
void Write2Nib(uint8_t chr)
{
	uint8_t HNib, LNib, temp_data;
	temp_data=DATA_O & 0x0F; //doc 4 bit thap cua DATA_O de mask,

	HNib=chr & 0xF0;
	LNib=(chr<<4) & 0xF0;

	DATA_O =(HNib |temp_data);
	sbi(CTRL,EN); //enable
	cbi(CTRL,EN); //disable

	DATA_O =(LNib|temp_data);
	sbi(CTRL,EN); //enable
	cbi(CTRL,EN); //disable
}
//------------------------------------------------------------------------------
void wait_LCD()
{
	_delay_us(1000);
}
//------------------------------------------------------------------------------
void init_LCD()
{
	DDR_CTRL=0xFF;
	sbi(DDR_DA,4);
	sbi(DDR_DA,5);
	sbi(DDR_DA,6);
	sbi(DDR_DA,7);
	//Function set------------------------------------------------------------------------------
	cbi(CTRL,RS);   // the following data is COMMAND
	
	cbi(CTRL, EN);
	
	sbi(CTRL,EN); //enable
	sbi(DATA_O, 5);
	cbi(CTRL,EN); //disable
	wait_LCD();
	Write2Nib(0x28);//4 bit mode, 2 line, 5x8 font
	wait_LCD();
	
	//Display control-------------------------------------------------------------------------
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x0C);
	wait_LCD();
	
	//Entry mode set------------------------------------------------------------------------
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x06);
	wait_LCD();
	
}
//------------------------------------------------------------------------------
void home_LCD()
{
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(0x02);
	wait_LCD();
	
}
//------------------------------------------------------------------------------
void move_LCD(uint8_t y,uint8_t x)
{
	uint8_t Ad;
	Ad=64*(y-1)+(x-1)+0x80; // tính mã l?nh
	cbi(CTRL,RS); // the following data is COMMAND
	
	Write2Nib(Ad);
	wait_LCD();
	
}
//------------------------------------------------------------------------------
void clr_LCD()
{ //xóa toàn b? LCD
	cbi(CTRL,RS); //RS=0 mean the following data is COMMAND (not normal DATA)
	
	Write2Nib(0x01);
	wait_LCD();
	
}
//------------------------------------------------------------------------------
void putChar_LCD(uint8_t chr)
{ //hi?n th? 1 ký t? chr lên LCD
	sbi(CTRL,RS); //this is a normal DATA
	Write2Nib(chr);
	wait_LCD();
}
//------------------------------------------------------------------------------
void print_LCD(char* str)
{
	int i;
	for (i=0; str[i]!=0; i++) 
	putChar_LCD(str[i]);
}
//------------------------------------------------------------------------------