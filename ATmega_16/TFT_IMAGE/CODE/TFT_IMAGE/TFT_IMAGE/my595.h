#define PORT_595 PORTC
#define DDR_595 DDRC
#define DS 5			//DATA
#define SH_CP 7			//SRC
#define ST_CP 6			//RCL
void Init_595()
{
	PORT_595|=(0<<DS)|(0<<ST_CP)|(0<<SH_CP);
	DDR_595|=(1<<DS)|(1<<ST_CP)|(1<<SH_CP);
}
void Ship_595(unsigned int data)
{
	for (int i=8;i>0;i--)
	{
		if((data&0x80)==0x80)
			setb(PORT_595,DS);
		else
			clrb(PORT_595,DS);
		data=data<<1;
		clrb(PORT_595,SH_CP);//xung clk cua thanh ghi dich
		setb(PORT_595,SH_CP);
	}
	clrb(PORT_595,ST_CP);//xung clk cua thanh ghi dich
	setb(PORT_595,ST_CP);
}