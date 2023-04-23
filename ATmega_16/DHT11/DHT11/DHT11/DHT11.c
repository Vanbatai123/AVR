/*
 * DHT11.c
 *
 * Created: 4/5/2016 9:27:40 AM
 *  Author: Administrator
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/interrupt.h>
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))
#include "E:\AVR\Header\myLCD_HP.h"
#define DHT_DDR DDRD
#define DHT_PORT PORTD
#define DHT_PIN PIND
#define DATA 0

int check(int *tem,int *humi)
{
	uint8_t buffer[5]={0,0,0,0,0};
	uint8_t ii,i,checksum;
	
	setb(DHT_DDR,DATA);   // set la cong ra
	setb(DHT_PORT,DATA);
	_delay_us(60);
	clrb(DHT_PORT,DATA);
	_delay_ms(25); // it nhat 18ms
	setb(DHT_PORT,DATA);
	clrb(DHT_DDR,DATA);
	_delay_us(60);
	if(inbit(DHT_PIN,DATA)==1)
		return 0;
	else while(inbit(DHT_PIN,DATA)==0);	//Doi DaTa len 1
	_delay_us(60);
	if(inbit(DHT_PIN,DATA)==0)return 0;
	else while(inbit(DHT_PIN,DATA)==1);	 //Doi Data ve 0
	//Bat dau doc du lieu
	for(i=0;i<5;i++)
	{
		for(ii=0;ii<8;ii++)
		{
			while(inbit(DHT_PIN,DATA)==0);//Doi Data len 1
			_delay_us(50);
			if(inbit(DHT_PIN,DATA)==1)
			{
				buffer[i]|=(1<<(7-ii));
				while(inbit(DHT_PIN,DATA)==1);//Doi Data xuong 0
			}
		}
	}
	//Tinh toan check sum
	checksum=buffer[0]+buffer[1]+buffer[2]+buffer[3];
	//Kiem tra check sum
	if((checksum)!=buffer[4])return 0;
	//Lay du lieu
	*tem  =   buffer[2];
	*humi =   buffer[0];
	return 1;
}
int main(void)
{
	//uint8_t str[20];
	int dht_nhiet_do=10,dht_do_am=20;
	init_LCD();
    while(1)
    {
        if(check(&dht_nhiet_do,&dht_do_am))
        {
			move_LCD(4,4);
			print_LCD("DO AM = ");
	        move_LCD(4,13);
	        vietso_LCD(dht_do_am);
			move_LCD(5,1);
			print_LCD("NHIET DO = ");
	        move_LCD(5,13);
	        vietso_LCD(dht_nhiet_do);
        }
        _delay_ms(300);
	}
}