/*
 * WAV_ARDUINO.c
 *
 * Created: 4/15/2016 4:15:20 PM
 *  Author: Administrator
 */ 
#include "C:\Users\Administrator\Desktop\WAV\SD.h"
//#define SD_ChipSelectPin 53  //s? d?ng SS Pin 53 trên Mega2560
#define SD_ChipSelectPin 10  //th??ng s? d?ng digital pin 4 trên arduino nano 328, ho?c chân tùy ý
//#include "C:\Users\Administrator\Desktop\WAV\TMRpcm.h"
//#include "C:\Users\Administrator\Desktop\WAV\SPI.h"
 #include "E:\AVR\Header\myUART.h"
TMRpcm tmrpcm;
unsigned long time = 0;
 
int main(void)
{
 /*
	  tmrpcm.loa = 1; //5,6,11 ho?c 46 trên Mega, 9 trên Uno, Nano. Vì sao, xem bài vi?t c?ng c?a anh NTP_PRO http://arduino.vn/bai-viet/411-timercounter-tren-avrarduino
 
	  //Serial.begin(9600); //b?t serial monitor ? m?c baudrate 9600 :)
	  if (!SD.begin(SD_ChipSelectPin))
	  {
		print_UART("SD fail");
		return;
	  }
	  else
	  {
		print_UART("OK");
	  }
  
	  tmrpcm.play("xinchao.wav");
	  _delay_ms(3000);
	  time = millis();
  */
	 while(1)
	{
	//  tmrpcm.play("aaa.wav");
	  _delay_ms(550000);
	 // digitalWrite(9, LOW);
 
	  /*if(millis() - time > 50)
	  {
		if (!tmrpcm.isPlaying())
		  digitalWrite(9, LOW);
		time = millis();
	  }*/
	}
 
}
 
 
 
