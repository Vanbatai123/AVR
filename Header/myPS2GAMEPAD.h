////////////////For Gamepad PS2/////////////////
short int  header0, header1, header2, data1, data2;
signed char joy_right_x, joy_right_y, joy_left_x, joy_left_y;
unsigned char chk_ana = 0;
////// Nut nhan PS2
#define PORT_GAMEPAD	PORTD
#define DDR_GAMEPAD		DDRD
#define CLK				4               // blue			3
#define DATA			5               // brown		6
#define CMD				6               // orange		5
#define ATT				7               // yelow		4

// PSx controller communication function.
// send a byte on the command line and receive one on the data line.
// needs Attention pin to have gone low before called to activate controller.
int gameByte(short int command)
{
	short int i ;
	_delay_us(1);
	short int data = 0x00;                             // clear data variable to save setting low bits later.
	for(i=0;i<8;i++)
	{
		if(command & _BV(i))
		setb(PORT_GAMEPAD, CMD);       // bit bang "command" out on CMD wire.
		else
		clrb(PORT_GAMEPAD, CMD);
		clrb(PORT_GAMEPAD, CLK);                             // CLOCK LOW
		_delay_us(1);                                              // wait for output to stabilise
		if((PIND & _BV(DATA)))
		setb(data, i);  // read DATA pin and store
		//else clrb(data, i);
		setb(PORT_GAMEPAD, CLK);                             // CLOCK HIGH
	}
	setb(PORT_GAMEPAD, CMD);

	_delay_us(20);                                                   // wait for ACK to pass.

	return(data);
}
void init_GAMEPAD()
{
	DDR_GAMEPAD = (1<<CLK)|(1<<CMD)|(1<<ATT)|(0<<DATA);
	PORT_GAMEPAD = 0xFF;

	while((chk_ana != 0x73)&&(chk_ana != 0x41))
	{
		// put controller in config mode
		setb(PORT_GAMEPAD, CMD);
		setb(PORT_GAMEPAD, CLK);
		clrb(PORT_GAMEPAD, ATT);

		gameByte(0x01);
		gameByte(0x43);
		gameByte(0x00);
		gameByte(0x01);
		gameByte(0x00);

		setb(PORT_GAMEPAD, CMD);
		_delay_ms(1);
		setb(PORT_GAMEPAD, ATT);

		_delay_ms(10);

		// put controller in analouge mode
		setb(PORT_GAMEPAD, CMD);
		setb(PORT_GAMEPAD, CLK);
		clrb(PORT_GAMEPAD, ATT);
		gameByte(0x01);
		gameByte(0x44);
		gameByte(0x00);
		gameByte(0x01);
		gameByte(0x03);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);
		setb(PORT_GAMEPAD, CMD);
		_delay_ms(1);
		setb(PORT_GAMEPAD, ATT);
		_delay_ms(10);
		// exit config mode
		setb(PORT_GAMEPAD, CMD);
		setb(PORT_GAMEPAD, CLK);
		clrb(PORT_GAMEPAD, ATT);
		gameByte(0x01);
		gameByte(0x43);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x5A);
		gameByte(0x5A);
		gameByte(0x5A);
		gameByte(0x5A);
		gameByte(0x5A);
		setb(PORT_GAMEPAD, CMD);
		_delay_ms(1);
		setb(PORT_GAMEPAD, ATT);
		_delay_ms(10);
		// poll controller and check in analouge mode.
		setb(PORT_GAMEPAD, CMD);
		setb(PORT_GAMEPAD, CLK);
		clrb(PORT_GAMEPAD, ATT);
		gameByte(0x01);
		chk_ana = gameByte(0x42);            // the 2nd byte to be returned from the controller should = 0x73 for "red" analouge controller.
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);
		gameByte(0x00);

		setb(PORT_GAMEPAD, CMD);
		_delay_ms(1);
		setb(PORT_GAMEPAD, ATT);
		_delay_ms(10);
		// keep increasing counter to be dispalyed untill PSx controller confirms it's in analouge mode.
	}
}
void read_GAMEPAD()
{
	setb(PORT_GAMEPAD, CMD);                        // start communication with PSx controller
	setb(PORT_GAMEPAD, CLK);
	clrb(PORT_GAMEPAD, ATT);

	header0 = gameByte(0x01);                       // bite 0. header.
	header1 = gameByte(0x42);                       // bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
	header2 = gameByte(0x00);                       // bite 2. header.

	data1 = gameByte(0x00);                         // bite 3. first data bite.
	data2 = gameByte(0x00);                         // bite 4.
	joy_right_x  = gameByte(0x00) - 128;                         // bite 5.
	joy_right_y  = gameByte(0x00) - 128;                         // bite 6.
	joy_left_x   = gameByte(0x00) - 128;                         // bite 7.
	joy_left_y   = gameByte(0x00) - 128;                         // bite 8.

	_delay_us(1);
	setb(PORT_GAMEPAD, CMD);                      // close communication with PSx controller
	_delay_us(1);
	setb(PORT_GAMEPAD, ATT);						// all done.
	
	move_LCD(1,12);									// 3 bit header
	vietso_hex_LCD(header0);
	move_LCD(2,12);
	vietso_hex_LCD(header1);
	move_LCD(3,12);
	vietso_hex_LCD(header2);
	
	move_LCD(1,1);
	vietso_hex_LCD(data1);
	move_LCD(2,1);
	vietso_hex_LCD(data2);
	move_LCD(3,1);
	vietso_dec_LCD(joy_left_x);
	space_LCD(3);
	move_LCD(4,1);
	vietso_dec_LCD(joy_left_y);
	space_LCD(3);
	move_LCD(5,1);
	vietso_dec_LCD(joy_right_x);
	space_LCD(3);
	move_LCD(6,1);
	vietso_dec_LCD(joy_right_y);
	space_LCD(3);
}