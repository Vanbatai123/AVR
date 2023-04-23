#define DATA			6
#define CLK				7
#define PORT_PS2		PORTD
#define PIN_PS2			PIND
#define DDR_PS2			DDRD

#define INTEL			0x03
#define STANDARD		0X00

#define WAIT_CLK_UP		while (inbit(PIN_PS2,CLK)  == 0)
#define WAIT_CLK_DOWN	while (inbit(PIN_PS2,CLK)  == 1)
#define WAIT_DATA_UP	while (inbit(PIN_PS2,DATA) == 0)
#define WAIT_DATA_DOWN	while (inbit(PIN_PS2,DATA) == 1)

// Khai bao bien
unsigned char i, mstat, status,mouse_mode;
signed char change_x, change_y, change_z;		// change position
signed int position_x, position_y,position_z;	//position

void gohi(int pin)
{
	switch (pin)
	{
		case DATA:
		setb(PORT_PS2,DATA);
		clrb(DDR_PS2,DATA);
		break;
		case CLK:
		setb(PORT_PS2,CLK);
		clrb(PORT_PS2,CLK);
		break;
	}//end swith
}//end gohi

void golo(int pin)
{
	switch (pin)
	{
		case DATA:
		clrb(PORT_PS2,DATA);
		setb(DDR_PS2,DATA);
		break;
		case CLK:
		clrb(PORT_PS2,CLK);
		setb(PORT_PS2,CLK);
		break;
	}//end switch
}//end golo

void mouse_write(unsigned char data)
{
	char parity = 1;
	// Set DATA and CLK pins in output mode
	gohi(DATA);
	gohi(CLK);
	_delay_us(300);
	golo(CLK);
	_delay_us(300);
	golo(DATA);
	_delay_us(10);
	// Start bit
	gohi(CLK);
	/* Wait for mouse to take control of clock); */
	WAIT_CLK_DOWN;
	/* Clock is low, and we are clear to send data */
	for (i=0; i < 8; i++) 
	{
		if (data & 0x01) 
			gohi(DATA);
		else
			golo(DATA);
		// Wait for clock cycle 
		WAIT_CLK_UP;
		WAIT_CLK_DOWN;
		parity = parity ^ (data & 0x01);
		data = data >> 1;
	}//end for
	// Parity bit
	if (parity)
		gohi(DATA);
	else
		golo(DATA);
	WAIT_CLK_UP;
	WAIT_CLK_DOWN;
	// stop bit 
	gohi(DATA);
	_delay_us(50);
	WAIT_CLK_DOWN;
	// Wait for mouse to switch modes
	while((inbit(PIN_PS2,CLK)==0) && (inbit(PIN_PS2,DATA)==0));
}//end mouse_write

signed char mouse_read(void)//Get a byte of data from the mouse
{
	signed  char data = 0x00;
	int i;
	char mask = 0x01;
	/* start the clock */
	gohi(CLK);
	gohi(DATA);
	_delay_us(50);

	WAIT_CLK_DOWN;
	WAIT_CLK_UP;
	for (i=0; i < 8; i++) 
	{
		WAIT_CLK_DOWN;
		if (inbit(PIN_PS2,DATA)==1) 
			data = data | mask;
		while (inbit(PIN_PS2,CLK) == 0);
		mask = mask << 1;
	}// end for
	// eat parity bit, which we ignore
	while (inbit(PIN_PS2,CLK)==1);
	while (inbit(PIN_PS2,CLK)==0);
	// eat stop bit
	while (inbit(PIN_PS2,CLK)==1);
	while (inbit(PIN_PS2,CLK)==0);
	return data;
}//end mouse_read

void mouse_standard()			//	Init mouse at standard mode
{
	mouse_mode = STANDARD;
	
	gohi(CLK);
	gohi(DATA);
	
	mouse_write(0xff);
	mouse_read();
	mouse_read();
	mouse_read();
	mouse_write(0xF0);
	mouse_read();
	/*mouse_write(0xf3);
	mouse_read();
	mouse_write(200);
	mouse_read();
	mouse_write(0xf3);
	mouse_read();
	mouse_write(100);
	mouse_read();
	mouse_write(0xf3);
	mouse_read();
	mouse_write(80);
	mouse_read();
	mouse_write(0xf2);
	mouse_read();
	mouse_read();*/
	_delay_us(100);
}

void mouse_intel()			//init mouse at intelligent mode
{
	mouse_mode = INTEL;
	
	gohi(CLK);
	gohi(DATA);
	
	/* 3 bit below maybe wait for first initialize*/
	mouse_write(0xFF);			// Reset mode
	mouse_read();				// Ack byte
	mouse_read();				// blank
	mouse_read();				// blank
	
	mouse_write(0xF3);			// Set rate command
	mouse_read();				// Ack
	mouse_write(0xC8);			// Set sample rate 200
	mouse_read();				// Ack
	
	mouse_write(0xF3);			// Set rate command
	mouse_read();				// Ack
	mouse_write(0x64);			// Set sample rate 100
	mouse_read();				// Ack
	
	mouse_write(0xF3);			// Set rate command
	mouse_read();				// Ack
	mouse_write(0x50);			// Set sample rate 80
	mouse_read();				// Ack
	
	mouse_write(0xF2);			// Read ID
	mouse_read();				// Ack
	mouse_read();				// mouse id, if this value is 0x00 mouse is standard, if it is 0x03 mouse is Intellimouse
	
/*	mouse_write(0xe8);			// Set wheel resolution
	mouse_read();				// Ack
	mouse_write(0x03);			// 8 counts per mm
	mouse_read();				// Ack
	
	mouse_write(0xe6);			// scaling 1:1
	mouse_read();				// Ack 
	
	mouse_write(0xf3);			// Set sample rate
	mouse_read();				// Ack 
	mouse_write(0x28);			// Set sample rate = 40
	mouse_read();				// Ack 
	
	mouse_write(0xf4);			// Enable device
	mouse_read();				// Ack 
	mouse_write(0xf0);			// Set Remote mode
	mouse_read();				// Ack					*/
	_delay_us(100);
	}


void mouse_all(void)/* get a reading from the mouse */
{
	mouse_write(0xEB);  /* give me data! */
	mouse_read();      /* ignore ack */
	mstat = mouse_read();
	change_x = mouse_read();
	change_y = mouse_read();
	if (mouse_mode)
		change_z = mouse_read();
	position_z+=change_z;
	position_x+=change_x;
	position_y+=change_y;
	_delay_ms(20);  /* twiddle */
}