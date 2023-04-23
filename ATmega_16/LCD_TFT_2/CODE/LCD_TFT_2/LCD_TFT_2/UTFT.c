#include  "UTFT.h"
unsigned int X_SIZE = 0,Y_SIZE = 0;
//font 16
const unsigned char chars16[][32] PROGMEM =
{
	
};
//-----------------------------------------------------------------------------------------------------------
void Port_Init()
{
	PORT_DATA=0x00;
	DDR_DATA=0xFF;
	DDR_CMD=0X1F;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_SendCommand(unsigned char cmd)
{
	CD_COMMAND;
	RD_IDLE;
	CS_ACTIVE;
	PORT_DATA=cmd;
	WR_STROBE;
	CS_IDLE;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_SendData(unsigned char dt)
{
	CD_DATA;
	RD_IDLE;
	CS_ACTIVE;
	PORT_DATA=dt;
	WR_STROBE;
	CS_IDLE;		
}
//-----------------------------------------------------------------------------------------------------------
void TFT_Write8(unsigned char dt)
{
	PORT_DATA=dt;
	WR_STROBE;
	
}
//-----------------------------------------------------------------------------------------------------------
unsigned long TFT_ReadReg(unsigned char r)
{
	unsigned long id;
	unsigned char x;
	CS_ACTIVE;
	CD_COMMAND;
	TFT_Write8(r);
	setReadDir();
	CD_DATA;
	_delay_us(50);
	RD_ACTIVE;
	_delay_us(5);
	x=PIN_DATA;
	RD_IDLE;
	id=x;
	id<<=8;
	RD_ACTIVE;
	_delay_us(5);
	x=PIN_DATA;
	RD_IDLE;
	id|=x;
	id<<=8;
	RD_ACTIVE;
	_delay_us(5);
	x=PIN_DATA;
	RD_IDLE;
	id|=x;
	id<<=8;
	RD_ACTIVE;
	_delay_us(5);
	x=PIN_DATA;
	RD_IDLE;
	id|=x;
	if (r==0xEF)
	{
		id<<=8;
		RD_ACTIVE;
		_delay_us(5);
		x=PIN_DATA;
		RD_IDLE;
		id|=x;
	}
	CS_IDLE;
	setWriteDir();
	_delay_us(150);//stabilization time
	return id;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_SetRotation(unsigned char r)
{
	TFT_SendCommand(0x36);
	switch(r)
	{
		case 0:
		TFT_SendData(0x48);
		X_SIZE = 240;
		Y_SIZE = 320;
		break;
		case 1:
		TFT_SendData(0x28);
		X_SIZE = 320;
		Y_SIZE = 240;
		break;
		case 2:
		TFT_SendData(0x88);
		X_SIZE = 240;
		Y_SIZE = 320;
		break;
		case 3:
		TFT_SendData(0xE8);
		X_SIZE = 320;
		Y_SIZE = 240;
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------
void TFT_Flood(unsigned short color,unsigned long len)
{
	unsigned short blocks;
	unsigned char i,hi = color>>8,lo=color;
	CS_ACTIVE;
	CD_COMMAND;
	TFT_Write8(0x2C);
	CD_DATA;
	TFT_Write8(hi);
	TFT_Write8(lo);
	len--;
	blocks=(unsigned short)(len/64);//64pitxel/block
	if (hi==lo)
	{
		while(blocks--)
		{
			i=16;
			do 
			{
				WR_STROBE;WR_STROBE;WR_STROBE;WR_STROBE;//2byte/pitxel
				WR_STROBE;WR_STROBE;WR_STROBE;WR_STROBE;//x4pitxel
			} 
			while (--i);
		}
		//Fill any remaining pixel(1 to 64)
		for (i=(unsigned char)len&63;i--;)
		{
			WR_STROBE;
			WR_STROBE;
		}
	}//end if
	else
	{
		while(blocks--)
		{
			i=16;
			do
			{
				TFT_Write8(hi);	TFT_Write8(lo);	TFT_Write8(hi);	TFT_Write8(lo);//2byte/pitxel
				TFT_Write8(hi);	TFT_Write8(lo);	TFT_Write8(hi);	TFT_Write8(lo);//x4pitxel
			}
			while (--i);
		}
		//Fill any remaining pixel(1 to 64)
		for (i=(unsigned char)len&63;i--;)
		{
			TFT_Write8(hi);
			TFT_Write8(lo);
		}
	}
	CS_IDLE;
	
}
//-----------------------------------------------------------------------------------------------------------
void TFT_Reset(void)
{
	CS_IDLE;
	WR_IDLE;
	RD_IDLE;
	RESET_ACTIVE;
	_delay_ms(2);
	RESET_IDLE;
	CS_ACTIVE;
	TFT_SendCommand(0x01);//Softwear Reset
	for (uint8_t i=0;i<3;i++)
		WR_STROBE;
	CS_IDLE;
	
}
//-----------------------------------------------------------------------------------------------------------
void TFT_WriteReg32(unsigned char r,unsigned long d)
{
	CS_ACTIVE;
	CD_COMMAND;
	TFT_Write8(r);
	CD_DATA;
	_delay_us(10);
	TFT_Write8(d>>24);
	_delay_us(10);
	TFT_Write8(d>>16);
	_delay_us(10);
	TFT_Write8(d>>8);
	_delay_us(10);
	TFT_Write8(d);
	CS_IDLE;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_SetAddrWindow(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	unsigned long t;
	CS_ACTIVE;
	t=x1;
	t<<=16;
	t|=x2;
	TFT_WriteReg32(0x2A,t);//Set Column
	t=y1;
	t<<=16;
	t|=y2;
	TFT_WriteReg32(0x2B,t);//Set Page
	CS_IDLE;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_FillScreen(unsigned int color)
{
	TFT_SetAddrWindow(0,0,X_SIZE-1,Y_SIZE-1);
	TFT_Flood(color,(long)X_SIZE*(long)Y_SIZE);
}
//-----------------------------------------------------------------------------------------------------------
void TFT_FillRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
	TFT_SetAddrWindow(x1,y1,x2,y2);
	TFT_Flood(color,(long)(x2-x1+1)*(long)(y2-y1+1));
}
//-----------------------------------------------------------------------------------------------------------
void TFT_DrawLineY(unsigned int x,unsigned int y1,unsigned int y2,unsigned int color)
{
	if ((x>=X_SIZE)|(y1>=Y_SIZE)|(y2>=Y_SIZE))
		return;
	if (y1>y2)
		swap(y1,y2);
	TFT_SetAddrWindow(x,y1,x,y2);
	TFT_Flood(color,(long)(y2-y1+1));
}
//-----------------------------------------------------------------------------------------------------------
void TFT_DrawLineX(unsigned int x1,unsigned int x2,unsigned int y,unsigned int color)
{
	if ((x1>=X_SIZE)|(x2>=X_SIZE)|(y>=Y_SIZE))
		return;
	if (x1>x2)
		swap(x1,x2);
	TFT_SetAddrWindow(x1,y,x2,y);
	TFT_Flood(color,(long)(x2-x1+1));
}
//-----------------------------------------------------------------------------------------------------------
void TFT_DrawCircle(unsigned int x,unsigned int y,unsigned int radius,unsigned int color)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int a1 = 0;
	int b1 = radius;
	TFT_DrawPixel(x,y+radius,color);
	TFT_DrawPixel(x,y-radius,color);
	TFT_DrawPixel(x+radius,y,color);
	TFT_DrawPixel(x-radius,y,color);
	while(a1 < b1)
	{
		if(f >= 0)
		{
			b1--;
			ddF_y += 2;
			f += ddF_y;
		}
		a1++;
		ddF_x += 2;
		f += ddF_x;
		TFT_DrawPixel(x + a1, y + b1, color);
		TFT_DrawPixel(x - a1, y + b1, color);
		TFT_DrawPixel(x + a1, y - b1, color);
		TFT_DrawPixel(x - a1, y - b1, color);
		TFT_DrawPixel(x + b1, y + a1,color);
		TFT_DrawPixel(x - b1, y + a1, color);
		TFT_DrawPixel(x + b1, y - a1, color);
		TFT_DrawPixel(x - b1, y - a1, color);
	}
}
//-----------------------------------------------------------------------------------------------------------
void TFT_DrawRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
	if ((x1>=X_SIZE)|(x2>=X_SIZE)|(y1>=Y_SIZE)|(y2>=Y_SIZE))
		return;
	TFT_DrawLineX(x1,x2,y1,color);
	TFT_DrawLineY(x2,y1,y2,color);
	TFT_DrawLineX(x1,x2,y2,color);
	TFT_DrawLineY(x1,y1,y2,color);
}
//-----------------------------------------------------------------------------------------------------------
unsigned int TFT_RandColor()//random
{
	unsigned char c = rand()%8;
	switch (c)
	{
		case 0:
			return BLACK;	
			break;	
		case 1:		
			return BLUE;			
			break;
		case 2:
			return RED;
			break;
		case 3:
			return GREEN;
			break;
		case 4:
			return CYAN;
			break;
		case 5:
			return MAGENTA;
			break;
		case 6:
			return YELLOW;
			break;
		case 7:
			return WHITE;
			break;
	}
	return BLACK;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_DrawPixel(int x,int y,unsigned int color)
{
	if ((x<0)||(y<0)||(x>=X_SIZE)||(y>=Y_SIZE))
		return;
	CS_ACTIVE;
	TFT_SetAddrWindow(x,y,X_SIZE-1,Y_SIZE-1);	
	CS_ACTIVE;
	CD_COMMAND;
	TFT_Write8(0x2C);
	CD_DATA;
	TFT_Write8(color>>8);
	TFT_Write8(color);
	CS_IDLE;
}
//-----------------------------------------------------------------------------------------------------------
void TFT_PutChar(unsigned int x, unsigned int y,unsigned char charr,unsigned char size, unsigned int color)
{
	//Small Font
	switch(size)
	{	
		case SMALL:
		for(int cot=0;cot<12;cot++)
		{
			for (int hang=0;hang<8;hang++)
			{
				if((pgm_read_byte(&SmallFont[charr-32][11-cot])<<hang)&0x80)
					TFT_DrawPixel(x+hang,y+cot,color);
				else
					TFT_DrawPixel(x+hang,y+cot,SCREEN_COLOR);
			}
		}
		break;
		//Big Font
		case BIG:
		for(int hang=0;hang<32;hang++)
		{
			if (hang%2==0)
			for (int cot=0;cot<8;cot++)
			{
				if((pgm_read_byte(&BigFont[charr-32][31-hang]))<<cot&0x80)
					TFT_DrawPixel(x+8+cot,y+hang/2,color);
				else
					TFT_DrawPixel(x+8+cot,y+hang/2,SCREEN_COLOR);
			}
			else
			for (int cot=0;cot<8;cot++)
			{
				if((pgm_read_byte(&BigFont[charr-32][31-hang]))<<cot&0x80)
					TFT_DrawPixel(x+cot,y+(hang-1)/2,color);
				else
					TFT_DrawPixel(x+cot,y+(hang-1)/2,SCREEN_COLOR);
			}
		}
		break;
	}
}

void TFT_Number_Hex(unsigned int x, unsigned int y,long number,unsigned char size, unsigned int color)
{
	char dis[10];
	sprintf(dis, "0x%lx", number);
	TFT_Print(x, y,dis,size,color);
}
//-----------------------------------------------------------------------------------------------------------
void TFT_Number_Dec(unsigned int x, unsigned int y,long number,unsigned char size, unsigned int color)
{
	char dis[10];
	sprintf(dis, "%li", number);
	TFT_Print(x, y,dis,size,color);
}
//-----------------------------------------------------------------------------------------------------------
void TFT_Print(unsigned int x, unsigned int y,char *text,unsigned char size, unsigned int color)
{
	while(*text)
	{
		TFT_PutChar(x,y,*text++,size,color);
		switch(size)
		{
			case BIG:
				x+=13;
				break;
			case SMALL:
				x+=7;
				break;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------
void TFT_Init(void)
{
	//char str[10];
	Port_Init();
	TFT_Reset();
	_delay_ms(100);
	//dtt=TFT_ReadReg(0xD3);
	CS_ACTIVE;
	/*move_LCD(1,1);
	sprintf(str,"0x%08lX",dtt);
	print_LCD(str);
	move_LCD(2,1);
	vietso_dec_LCD(dtt);*/
	TFT_SendCommand(0x01);//software reset
	TFT_SendCommand(0xCB);//power ctrl A
	TFT_SendData(0x39);
	TFT_SendData(0x2C);
	TFT_SendData(0x00);
	TFT_SendData(0x34);
	TFT_SendData(0x02);
	TFT_SendCommand(0xCF);// power ctrl B
	TFT_SendData(0x00);
	TFT_SendData(0xC1);
	TFT_SendData(0x30);
	TFT_SendCommand(0xE8);//driver timing A
	TFT_SendData(0x85);
	TFT_SendData(0x00);
	TFT_SendData(0x78);
	TFT_SendCommand(0xEA);//driver B
	TFT_SendData(0x00);
	TFT_SendData(0x00);
	TFT_SendCommand(0xED);//power on
	
	TFT_SendData(0x64);
	TFT_SendData(0x03);
	TFT_SendData(0x12);
	TFT_SendData(0x81);
	TFT_SendCommand(0xF7);//pump ratio ctrl
	TFT_SendData(0x20);
	TFT_SendCommand(0xC0);//power ctr1
	TFT_SendData(0x10);
	TFT_SendCommand(0xC1);//power ctr2
	TFT_SendData(0x10);
	TFT_SendCommand(0xC5);//VCOM ctrl1
	TFT_SendData(0x3E);
	TFT_SendData(0x28);
	TFT_SendCommand(0xC7);//VCOM ctrl2
	TFT_SendData(0x86);
	TFT_SetRotation(1);
	TFT_SendCommand(0x3A);//Pixel Format
	TFT_SendData(0x55);//16bit
	TFT_SendCommand(0xB1);
	TFT_SendData(0x00);
	TFT_SendData(0x18);//
	TFT_SendCommand(0xB6);//Display Function ctrl
	TFT_SendData(0x08);
	TFT_SendData(0x82);//
	TFT_SendData(0x27);//320
	TFT_SendCommand(0xF2);//Enable 3G
	TFT_SendData(0x00);
	TFT_SendCommand(0x26);//Gamma Curve
	TFT_SendData(0x01);
	TFT_SendCommand(0xE0);//Position Gamma  Curve
	TFT_SendData(0x0F);
	TFT_SendData(0x31);
	TFT_SendData(0x2B);
	TFT_SendData(0x0C);
	TFT_SendData(0x0E);
	TFT_SendData(0x08);
	TFT_SendData(0x4E);
	TFT_SendData(0xF1);
	TFT_SendData(0x37);
	TFT_SendData(0x07);
	TFT_SendData(0x10);
	TFT_SendData(0x03);
	TFT_SendData(0X0E);
	TFT_SendData(0X09);
	TFT_SendData(0x00);
	TFT_SendCommand(0xE1);//Negative Gamma  Correct
	TFT_SendData(0x00);
	TFT_SendData(0x0E);
	TFT_SendData(0x14);
	TFT_SendData(0x03);
	TFT_SendData(0x11);
	TFT_SendData(0x07);
	TFT_SendData(0x31);
	TFT_SendData(0xC1);
	TFT_SendData(0x48);
	TFT_SendData(0x08);
	TFT_SendData(0x0F);
	TFT_SendData(0x0C);
	TFT_SendData(0x31);
	TFT_SendData(0x36);
	TFT_SendData(0x0F);
	TFT_SendCommand(0X11);
	_delay_ms(150);
	TFT_SendCommand(0X29);
	TFT_SendData(0x2C);
	_delay_ms(150);
	
}
//-----------------------------------------------------------------------------------------------------------