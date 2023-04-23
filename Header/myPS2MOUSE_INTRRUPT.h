#define DATA 2
#define CLK 3
#define PORT_BJT PORTB
#define PIN_BJT PINB
#define DDR_BJT DDRB
#define PORT_PS2 PORTD
#define DDR_PS2 DDRD
#define PIN_PS2 PIND

#define UP_CLK			clrb(PORT_BJT,CLK)
#define DOWN_CLK		setb(PORT_BJT,CLK)
#define UP_DATA			clrb(PORT_BJT,DATA)
#define DOWN_DATA		setb(PORT_BJT,DATA)

#define WAIT_CLK_UP		while(inbit(PIN_PS2,CLK)==0)
#define WAIT_CLK_DOWN	while(inbit(PIN_PS2,CLK)==1)
#define WAIT_DATA_UP	while(inbit(PIN_PS2,DATA)==0)
#define WAIT_DATA_DOWN	while(inbit(PIN_PS2,DATA)==1)

int i,dembit=11,data=0;

void host_send(int byte)
{
	UP_CLK;//CLK=1
	UP_DATA;			//DATA=1
	char parity=1;		//parity bit
	clrb(GICR,INT1);	//disable interrupt 1
	DOWN_CLK;			//CLK=0
	_delay_us(500);
	DOWN_DATA;			//data=0
	UP_CLK;				//clk=1
	WAIT_CLK_DOWN;
	for (i=0;i<8;i++)	//send 8 bit data
	{
		if ((byte>>i)&0x01)
		{
			UP_DATA;	//data=1
			parity ^= 0x01;
		}
		else
			DOWN_DATA;	//data=0
		WAIT_CLK_UP;
		WAIT_CLK_DOWN;
	}					//end send 8 bit data
	if (parity)			//set/reset parity bit
		UP_DATA;		//data=1
	else
		DOWN_DATA;		//data=0
	WAIT_CLK_UP;
	WAIT_CLK_DOWN;
	UP_DATA;			//data=1 (stop bit)
	_delay_us(50);
	WAIT_DATA_DOWN;
	WAIT_CLK_DOWN;
	while((inbit(PIN_PS2,CLK)==0)&&(inbit(PIN_PS2,DATA)==0));//wait CLK UP and DATA UP
	dembit=11;
	data=0;
	setb(GICR,INT1);	//enable interrupt 1
	//_delay_ms(400);
}
void mouse_init()
{
	host_send(0xff);
	//mouse_read();  /* ack byte */
	//mouse_read();  /* blank */
	//mouse_read();  /* blank */
	host_send(0xf0);  /* remote mode */
	//mouse_read();  /* ack */
	_delay_us(100);
}