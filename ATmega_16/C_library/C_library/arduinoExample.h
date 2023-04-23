/*
 * arduinoExample.c
 *
 * Created: 11/2/2018 8:56:57 PM
 *  Author: PC
 */ 
#include <Wire.h>

/* ??a ch? c?a DS1307 */
const byte DS1307 = 0x68;
/* S? byte d? li?u s? ??c t? DS1307 */
const byte NumberOfFields = 7;

/* khai báo các bi?n th?i gian */
int second, minute, hour, day, wday, month, year;

void setup()
{
	Wire.begin();
	/* cài ??t th?i gian cho module */
	setTime(12, 30, 45, 1, 8, 2, 15); // 12:30:45 CN 08-02-2015
	Serial.begin(9600);
}

void loop()
{
	/* ??c d? li?u c?a DS1307 */
	readDS1307();
	/* Hi?n th? th?i gian ra Serial monitor */
	digitalClockDisplay();
	delay(1000);
}

void readDS1307()
{
	Wire.beginTransmission(DS1307);
	Wire.write((byte)0x00);
	Wire.endTransmission();
	Wire.requestFrom(DS1307, NumberOfFields);
	
	second = bcd2dec(Wire.read() & 0x7f);
	minute = bcd2dec(Wire.read() );
	hour   = bcd2dec(Wire.read() & 0x3f); // ch? ?? 24h.
	wday   = bcd2dec(Wire.read() );
	day    = bcd2dec(Wire.read() );
	month  = bcd2dec(Wire.read() );
	year   = bcd2dec(Wire.read() );
	year += 2000;
}
/* Chuy?n t? format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
	return ((num/16 * 10) + (num % 16));
}
/* Chuy?n t? Decimal sang BCD */
int dec2bcd(byte num)
{
	return ((num/10 * 16) + (num % 10));
}

void digitalClockDisplay(){
	// digital clock display of the time
	Serial.print(hour);
	printDigits(minute);
	printDigits(second);
	Serial.print(" ");
	Serial.print(day);
	Serial.print(" ");
	Serial.print(month);
	Serial.print(" ");
	Serial.print(year);
	Serial.println();
}

void printDigits(int digits){
	// các thành ph?n th?i gian ???c ng?n chách b?ng d?u :
	Serial.print(":");
	
	if(digits < 10)
	Serial.print('0');
	Serial.print(digits);
}

/* cài ??t th?i gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
	Wire.beginTransmission(DS1307);
	Wire.write(byte(0x00)); // ??t l?i pointer
	Wire.write(dec2bcd(sec));
	Wire.write(dec2bcd(min));
	Wire.write(dec2bcd(hr));
	Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
	Wire.write(dec2bcd(d));
	Wire.write(dec2bcd(mth));
	Wire.write(dec2bcd(yr));
	Wire.endTransmission();
}