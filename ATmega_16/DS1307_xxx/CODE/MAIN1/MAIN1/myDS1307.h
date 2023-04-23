/*
 * myDS1307.h
 *
 * Created: 4/20/2017 08:54:57 PM
 *  Author: Van_BasTai
 */ 
/****************************PORT, DDR*************************************/
#define I2C_PORT		PORTD
#define I2C_DDR			DDRD
#define I2C_PIN			PIND
/******************************PIN*****************************************/
#define SDA				1
#define SCL				0
/************************Marco can thiet***********************************/
#define write_data		setb(I2C_DDR, SDA)
#define read_data		setb(I2C_PORT, SDA);clrb(I2C_DDR, SDA)
#define HIGH_SCL		setb(I2C_PORT, SCL)
#define LOW_SCL			clrb(I2C_PORT, SCL)
#define HIGH_SDA		setb(I2C_PORT, SDA)
#define LOW_SDA			clrb(I2C_PORT, SDA)
/********************Dia chi thanh ghi DS1307******************************/
#define SECOND			0x00// Giay
#define MINUTE			0x01// Phut
#define HOUR			0x02// Gio
#define DAY				0x03// Thu
#define DATE			0x04// Ngay
#define MONTH			0x05// Thang
#define YEAR			0x06// Nam
/*************************kieu du lieu**************************************/
#define uchar			unsigned char
/**********************Cac bien can thiet***********************************/
char count, second, minute, hour, date, month, year, day;
/********************Cac ham I2C va DS1307**********************************/
void i2c_start();//Ham khoi dong I2C
void i2c_stop();//Ham dung I2C
void i2c_write(int data);//Ham viet 1 byte I2C
uchar i2c_read();//Ham doc 1 byte I2C
void i2c_write_ack();//Ham viet bit acknowledge
void i2c_write_notack();//Ham viet bit not-acknowledge
uchar DS1307_read(char word_add);//Ham doc gia tri 1 word cua DS1307
void DS1307_write(char word_add, char word_value);//Ham viet gia tri vao 1 word cua DS1307
void DS1307_setup(char day, char date, char month, char year, char hour, char minute, char second);//Ham cai dat DS1307
void DS1307_get();//Ham lay gia tri vao cac bien day, date, month, year, hour, minute, second
void DS1307_init();//Ham khoi dong DS1307
void DS1307_display();//Ham hien thi gia tri len LCD
/****************************************************************************/
void i2c_start()
{
	write_data;
	HIGH_SCL;
	HIGH_SDA;
	LOW_SDA;
	LOW_SCL;
}
/****************************************************************************/
void i2c_stop()
{
	write_data;
	HIGH_SCL;
	LOW_SDA;
	HIGH_SDA;
	LOW_SCL;
}
/****************************************************************************/
void i2c_write(int data)
{
	write_data;
	for (count = 0; count < 8; count++)
	{
		if ((data & 0x80) == 0x80)
		HIGH_SDA;
		else
		LOW_SDA;
		HIGH_SCL;
		LOW_SCL;
		data = data<<1;
	}
	read_data;// Doc acknowledge
	HIGH_SCL;
	LOW_SCL;
}
/****************************************************************************/
unsigned char i2c_read()
{
	read_data;
	unsigned char dataa = 0;
	for (count = 0; count < 8; count++)
	{
		if (inbit(I2C_PIN, SDA))
		dataa = (dataa<<1) + 1;
		else
		dataa = dataa<<1;
		HIGH_SCL;
		LOW_SCL;
	}
	return dataa;
}
/****************************************************************************/
void i2c_write_ack()
{
	write_data;
	HIGH_SCL;
	LOW_SDA;
	LOW_SCL;
}
/****************************************************************************/
void i2c_write_notack()
{
	write_data;
	HIGH_SCL;
	HIGH_SDA;
	LOW_SCL;
}
/****************************************************************************/
uchar DS1307_read(char word_add)
{
	i2c_start();// Start
	i2c_write(0xD0);// DS1307 Address + Write to write word address
	i2c_write(word_add);// write word address
	i2c_start();// Start again
	i2c_write(0xD1);// DS1307 address + read to read word value
	char word_value = i2c_read();// Read word value
	i2c_write_notack();// Send not acknowledge
	i2c_stop();// Stop
	word_value = (word_value&0x0F) + 10 * (word_value>>4);// Hex to BCD
	return word_value;
}
/****************************************************************************/
void DS1307_write(char word_add, char word_value)
{
	word_value = ((word_value/10)<<4)|(word_value%10);
	i2c_start();
	i2c_write(0xD0);
	i2c_write(word_add);
	i2c_write(word_value);
	i2c_stop();
}
/****************************************************************************/
void DS1307_setup(char day, char date, char month, char year, char hour, char minute, char second)
{
	DS1307_write(SECOND, second);
	DS1307_write(MINUTE, minute);
	DS1307_write(HOUR, hour);
	DS1307_write(DATE, date);
	DS1307_write(MONTH, month);
	DS1307_write(YEAR, year);
	DS1307_write(DAY, day - 1);
}
/****************************************************************************/
void DS1307_get()
{
	second = DS1307_read(SECOND);
	minute = DS1307_read(MINUTE);
	hour = DS1307_read(HOUR);
	date = DS1307_read(DATE);
	month = DS1307_read(MONTH);
	year = DS1307_read(YEAR);
	day = DS1307_read(DAY);
}
/****************************************************************************/
void DS1307_init()
{
	I2C_PORT|=0<<SCL;
	I2C_DDR|=1<<SCL;
	DS1307_write(0x07, 0x12);
}
/****************************************************************************/
void DS1307_display()
{
	move_LCD(1, 1);
	vietso_LCD(date);
	move_LCD(1, 3);
	putChar_LCD('-');
	vietso_LCD(month);
	move_LCD(1, 6);
	putChar_LCD('-');
	vietso_LCD(year);
	move_LCD(2, 1);
	vietso_LCD(hour);
	move_LCD(2, 3);
	putChar_LCD('-');
	vietso_LCD(minute);
	move_LCD(2, 6);
	putChar_LCD('-');
	vietso_LCD(second);
	//move_LCD(2, 9);
	//putChar_LCD('-');
	//vietso_LCD(day + 1);
}