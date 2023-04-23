/*
* TestCpp.cpp
*
* Created: 7/25/2018 4:06:03 PM
* Author : taivb
*/

#include "include.h"

int main(void)
{
	HardwareSerial Serial;
	LiquidCrystal_I2C lcd(0x3F, 16, 2, LCD_5x10DOTS);
	lcd.begin();
	lcd.backlight();
	lcd.blink();
	Serial.begin(19200);
	while(1)
	{
		Serial.println("VAN BA TAI");
		lcd.setCursor(0, 0);
		lcd.print("HELLO!!!!!!");
		delay(1000);
	}
}