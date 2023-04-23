/*
 * QUADRUPED.c
 *
 * Created: 10/15/2016 11:51:47 AM
 *  Author: Administrator
 */ 


#define __DELAY_BACKWARD_COMPATIBLE__// mo khoa ham delay ???????????????
#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myTIMER_1.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myQUADRUPED.h"
#include "F:\AVR\Header\mySRF05.h"
int main(void)
{
	init_QUADRUPED();dung4chan();
	init_SRF05();	
	init_LCD();
	hesochia(8);
	
	depa();
	SRF05();
	//move();
	while(1)
    {	
		if (SRF05()>300)
		{
			move();
		}
		else
		{
			turnright();
			turnright();
			for (int q=0;q<5;q++)
			{
				move();
			}
			for (int w=0;w<2;w++)
			{
				turnleft();
			}
			for (int qw=0;qw<8;qw++)
			{
				move();
			}
			for (int w=0;w<3;w++)
			{
				turnleft();
			}
			for (int qw=0;qw<8;qw++)
			{
				move();
			}
			while(1)
				move2();
		}
	}
}