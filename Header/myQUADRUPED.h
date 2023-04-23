#define SERVO_DDR DDRA//Dinh nghia ddr va port cua servo
#define SERVO_PORT PORTA
void init_QUADRUPED()
{
	SERVO_DDR=0xFF;
}
void servo_0(int leg,float goc)
{
	goc=11*goc+500;
	setb(SERVO_PORT,leg);
	_delay_us(goc);
	clrb(SERVO_PORT,leg);
	_delay_us(20000-goc);
}
void servo_1(int leg,float goc1,int goc2,int goc3)//ham dieu khien 1 chan
{
	for(int x=0;x<15;x++)
	{
		servo_0(leg,goc1);
	}
	for(int x=0;x<15;x++)
	{
		servo_0(leg+1,goc2);
	}
	for(int x=0;x<10;x++)
	{
		servo_0(leg,goc3);
	}		
}
void servo_2(int servo1,float goc1,int servo2,float goc2)
{
	goc1=11*goc1+500;
	goc2=11*goc2+500;
	setb(SERVO_PORT,servo1);
	setb(SERVO_PORT,servo2);
	_delay_us(goc1);
	clrb(SERVO_PORT,servo1);
	_delay_us(goc2-goc1);
	clrb(SERVO_PORT,servo2);
	_delay_us(20000-goc2);
}
void cungvung()
{
	for(int x=0;x<10;x++)
	{
		servo_2(0,40,2,40);
		servo_2(4,143,6,143);
	}
}
void dung4chan()
{
	for (int x=0;x<20;x++)
	{
		servo_2(1,45,3,45);
		servo_2(5,135,7,135);
	}
	for(int x=0;x<20;x++)
	{
		servo_2(0,40,2,40);
		servo_2(4,143,6,143);
	}
}
void depa()
{
	servo_1(4,110,50,143);//chan 4
	servo_1(6,110,180,143);//chan 6
	servo_1(4,110,135,143);	//chan 4
	servo_1(2,60,90,40);//chan 2
	for(int x=0;x<10;x++)
	{
		servo_2(1,90,3,120);
		servo_2(5,60,7,90);
	}
	cungvung();
}
void move()
{
	//2 chan trai
	servo_1(0,60,10,40);//chan 0
	servo_1(2,70,40,40);//chan 2
	for(int x=0;x<10;x++)
	{
		servo_2(1,65,3,105);
		servo_2(5,45,7,70);
		/*servo_2(0,45,2,45);
		servo_2(4,135,6,135);*/
	}
	cungvung();
	//2 chan phai
	servo_1(6,110,180,143);//chan 6
	servo_1(4,110,150,143);//chan 4
	for(int x=0;x<10;x++)
	{
		servo_2(1,90,3,122);
		servo_2(5,63,7,90);
		/*servo_2(0,45,2,45);
		servo_2(4,135,6,135);*/
	}
	cungvung();
}
void move2()
{
	//2 chan trai
	servo_1(0,100,10,40);//chan 0
	servo_1(2,110,40,40);//chan 2
	for(int x=0;x<10;x++)
	{
		servo_2(1,65,3,105);
		servo_2(5,45,7,70);
		/*servo_2(0,45,2,45);
		servo_2(4,135,6,135);*/
	}
	cungvung();
	//2 chan phai
	servo_1(6,70,180,143);//chan 6
	servo_1(4,70,150,143);//chan 4
	for(int x=0;x<10;x++)
	{
		servo_2(1,90,3,122);
		servo_2(5,63,7,90);
		/*servo_2(0,45,2,45);
		servo_2(4,135,6,135);*/
	}
	cungvung();
}
void turnright()
{
	//chan trai
	//servo_1(0,60,10,40);//chan 0
	//servo_1(2,60,40,40);//chan 2
	for(int x=0;x<10;x++)
	{
		servo_2(1,65,3,110);
		servo_2(5,40,7,70);
	}
	cungvung();
	//2 chan phai
	servo_1(6,110,180,143);//chan 6
	servo_1(4,110,150,143);//chan 4
	for(int x=0;x<10;x++)
	{
		servo_2(1,90,3,135);
		servo_2(5,90,7,110);
	}
	cungvung();
	
}
void turnleft()
{
	//chan trai
	servo_1(0,60,10,40);//chan 0
	servo_1(2,60,40,40);//chan 2
	for(int x=0;x<10;x++)
	{
		servo_2(1,65,3,110);
		servo_2(5,40,7,70);
	}
	cungvung();
	//2 chan phai
	//servo_1(6,110,110,143);//chan 6
	//servo_1(4,110,90,143);//chan 4
	for(int x=0;x<10;x++)
	{
		servo_2(1,90,3,135);
		servo_2(5,90,7,110);
	}
	cungvung();
	
}