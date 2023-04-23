/*
* CNC_v1.16.c
*
* Created: 10/27/2018 10:14:53 PM
*  Author: Van_BasTai
*/

#include "include.h"
#include "STEPper/STEPper.h"
#include "UART/UART.h"
#include "Gcode/gcode.h"

#define DEBUGMODE
#define STEP 1

#define CNC_LINEAR_FAST	0
#define CNC_LINEAR_FEED	1
#define CNC_CIRCLE_CLW	2
#define CNC_CIRCLE_ACLW	3

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
char uartBuffer[50];

uint8_t transmitted = falsee;
uint8_t indexUART = 0;

struct point
{
	float x;
	float y;
	float angle;
};

struct point draw(struct gValue gVal1, struct point pntStart)
{
	
	// goc va delta goc dung cho G02 G03
	float angle;
	float deltaC;

	struct point pntCenter, pntEnd;
	struct point pntCur, pntPre;
	
	pntPre.x = pntStart.x;
	pntPre.y = pntStart.y;

	int32_t anglePre;
	// diem cuoi cung cua lenh
	pntEnd.x = gVal1.X;
	pntEnd.y = gVal1.Y;

	// neu 2 diem trung nhau thi bo qua

	#warning "them phan tra ve ma loi"
	if (pntStart.x == pntEnd.x && pntStart.y == pntEnd.y) return  pntStart;

	if (gVal1.G == CNC_LINEAR_FAST || gVal1.G == CNC_LINEAR_FEED)// noi suy tuyen tinh
	{
		// struct line dung cho G00 va G01
		struct line
		{
			float a, b, x, y, angle;
		}line1;

		// bien dem delta dung cho G00, G01
		int32_t deltaL;

		// goc cua duong thang
		line1.angle = 180 * atan2(pntEnd.y - pntStart.y, pntEnd.x - pntStart.x) / M_PI;
		if ((line1.angle > 45 && line1.angle < 135) || (line1.angle < -45 && line1.angle > -135))// duong thang x = Ay + B
		{
			// tim phuong trinh duong thang
			line1.a = (pntEnd.x - pntStart.x) / (pntEnd.y - pntStart.y);// 2/3
			line1.b = (pntEnd.x - pntEnd.y * line1.a);//
			line1.y = pntStart.y;
			
			if (pntStart.y <= pntEnd.y)
			{
				for (deltaL = line1.y; deltaL <= pntEnd.y; deltaL++)
				{
					pntCur.x = (int32_t)(line1.a * deltaL + line1.b);
					pntCur.y = deltaL;
					stepper_ctrl(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
					if (deltaL % 100 == 0)
					{
						UART_printXY(pntCur.x, pntCur.y);
					}
					pntPre.x = pntCur.x;
					pntPre.y = pntCur.y;
				}
			}
			else if (pntStart.y > pntEnd.y)
			{
				for (deltaL = line1.y; deltaL >= pntEnd.y; deltaL--)
				{
					pntCur.x = (int32_t)(line1.a * deltaL + line1.b);
					pntCur.y = deltaL;
					stepper_ctrl(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
					if (deltaL % 100 == 0)
					{
						UART_printXY(pntCur.x, pntCur.y);
					}
					pntPre.x = pntCur.x;
					pntPre.y = pntCur.y;
				}
			}
		}
		else// duong thang y = ax + b
		{
			// tim phuong trinh duong thang
			line1.a = (pntEnd.y - pntStart.y) / (pntEnd.x - pntStart.x);
			line1.b = pntEnd.y - pntEnd.x * line1.a;
			line1.x = pntStart.x;
			
			if (pntStart.x <= pntEnd.x)
			{
				for (deltaL = line1.x; deltaL <= pntEnd.x; deltaL++)
				{
					pntCur.x = deltaL;
					pntCur.y = (int32_t)(line1.a * deltaL + line1.b);
					//UART_printXY(pntCur.x, pntCur.y);
					//UART_printXY(pntPre.x, pntPre.y);
					//UART_printXY(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
					stepper_ctrl(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
					if (deltaL % 100 == 0)
					{
						UART_printXY(pntCur.x, pntCur.y);
					}
					pntPre.x = pntCur.x;
					pntPre.y = pntCur.y;
				}
			}
			else if (pntStart.x > pntEnd.x)
			{
				for (deltaL = line1.x; deltaL >= pntEnd.x; deltaL--)
				{
					pntCur.x = deltaL;
					pntCur.y = (int32_t)(line1.a * deltaL + line1.b);
					stepper_ctrl(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
					if (deltaL % 100 == 0)
					{
						UART_printXY(pntCur.x, pntCur.y);
					}
					pntPre.x = pntCur.x;
					pntPre.y = pntCur.y;
				}
			}
		}
	}
	else if (gVal1.G == CNC_CIRCLE_CLW)
	{
		// tim ban kinh duong tron
		if (gVal1.R == 0)
		{
			pntCenter.x = pntStart.x + gVal1.I;
			pntCenter.y = pntStart.y + gVal1.J;
			gVal1.R = sqrt((pntStart.x - pntCenter.x) * (pntStart.x - pntCenter.x) + (pntStart.y - pntCenter.y) * (pntStart.y - pntCenter.y));
		}
		else
		{
			// khoang cach diem bat dau va ket thuc
			float q = sqrt((pntEnd.x - pntStart.x) * (pntEnd.x - pntStart.x) + (pntEnd.y - pntStart.y) * (pntEnd.y - pntStart.y));
			pntCenter.x = (pntStart.x + pntEnd.x) / 2 - sqrt(gVal1.R * gVal1.R - (q * q / 4)) * (pntStart.y - pntEnd.y) / q;
			pntCenter.y = (pntStart.y + pntEnd.y) / 2 - sqrt(gVal1.R * gVal1.R - (q * q / 4)) * (pntEnd.x - pntStart.x) / q;
		}
		// khoang cach 2 diem lon hon 2 lan ban kinh
		if (sqrt((pntStart.x - pntCenter.x) * (pntStart.x - pntCenter.x) + (pntStart.y - pntCenter.y) * (pntStart.y - pntCenter.y)) > (2 * gVal1.R))
		{
			return pntStart;
		}
		// tim goc cua diem dau va diem cuoi so voi tam
		pntStart.angle = 180 * atan2(pntStart.y - pntCenter.y, pntStart.x - pntCenter.x) / M_PI;
		pntEnd.angle = 180 * atan2(pntEnd.y - pntCenter.y, pntEnd.x - pntCenter.x) / M_PI;
		
		// xac dinh goc de chay het cung tron
		angle = pntStart.angle;
		anglePre = (int)angle;
		
		// do dich chuyen cua 1 truc tuong ung voi 1 buoc (STEP)
		deltaC = 360 * STEP / (2 * M_PI * gVal1.R);
		// quen me roi????
		if ((pntStart.angle <= 0 && pntEnd.angle <= 0 && pntEnd.angle >= pntStart.angle)
		|| (pntStart.angle <= 0 && pntEnd.angle >= 0)
		|| (pntStart.angle >= 0 && pntEnd.angle >= 0 && pntEnd.angle >= pntStart.angle)
		)
		{
			pntEnd.angle -= 360;
		}
		while (angle >= pntEnd.angle)
		{
			pntCur.x = (int32_t)(gVal1.R * cos(angle * M_PI / 180) + pntCenter.x);
			pntCur.y = (int32_t)(gVal1.R * sin(angle * M_PI / 180) + pntCenter.y);
			//UART_printXY(pntCur.x, pntCur.y);
			//UART_printXY(pntPre.x, pntPre.y);
			//UART_printXY(pntCur.x - pntPre.x, pntCur.y -pntPre.y);
			stepper_ctrl(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
			if ((int)angle !=anglePre)// ghi gia tri len uart
			{
				//UART_printXY(pntCur.x, pntCur.y);
				//anglePre = (int)angle;
			}
			pntPre.x = pntCur.x;
			pntPre.y = pntCur.y;
			angle -= deltaC;
		}
		// reset gVal1.R = 0 for next Gcode
		gVal1.R = 0;
	}
	else if (gVal1.G == CNC_CIRCLE_ACLW)// G03 noi suy tron nguoc chieu kim dong ho
	{
		// tim tam duong tron
		if (gVal1.R == 0)
		{
			pntCenter.x = pntStart.x + gVal1.I;
			pntCenter.y = pntStart.y + gVal1.J;
			gVal1.R = sqrt((pntStart.x - pntCenter.x) * (pntStart.x - pntCenter.x) + (pntStart.y - pntCenter.y) * (pntStart.y - pntCenter.y));
		}
		else
		{
			//khoang cach diem dau va diem ket thuc
			float q = sqrt((pntEnd.x - pntStart.x) * (pntEnd.x - pntStart.x) + (pntEnd.y - pntStart.y) * (pntEnd.y - pntStart.y));
			// toa do diem tam
			pntCenter.x = (pntStart.x + pntEnd.x) / 2 + sqrt(gVal1.R * gVal1.R - (q * q / 4)) * (pntStart.y - pntEnd.y) / q;
			pntCenter.y = (pntStart.y + pntEnd.y) / 2 + sqrt(gVal1.R * gVal1.R - (q * q / 4)) * (pntEnd.x - pntStart.x) / q;
		}

		// khoang cach 2 diem lon hon 2 lan ban kinh
		if (sqrt((pntStart.x - pntCenter.x) * (pntStart.x - pntCenter.x) + (pntStart.y - pntCenter.y) * (pntStart.y - pntCenter.y)) > (2 * gVal1.R))
		{
			return pntStart;
		}

		// tim goc cua diem dau va diem cuoi so voi tam
		pntStart.angle = 180 * atan2(pntStart.y - pntCenter.y, pntStart.x - pntCenter.x) / M_PI;
		pntEnd.angle = 180 * atan2(pntEnd.y - pntCenter.y, pntEnd.x - pntCenter.x) / M_PI;
		
		// xac dinh goc de chay het cung tron
		angle = pntStart.angle;
		anglePre = (int)angle;

		// do dich chuyen cua 1 truc tuong ung voi 1 buoc (STEP)
		deltaC = 360 * STEP / (2 * M_PI * gVal1.R);
		// quen me roi, nhung ma cai nay khac cai case tren.
		if ((pntStart.angle <= 0 && pntEnd.angle <= 0 && pntEnd.angle <= pntStart.angle)
		|| (pntStart.angle >= 0 && pntEnd.angle <= 0)
		|| (pntStart.angle >= 0 && pntEnd.angle >= 0 && pntEnd.angle <= pntStart.angle)
		)
		{
			pntEnd.angle += 360;
		}
		while (angle <= pntEnd.angle)
		{
			pntCur.x = (int32_t)(gVal1.R * cos(angle * M_PI / 180) + pntCenter.x);
			pntCur.y = (int32_t)(gVal1.R * sin(angle * M_PI / 180) + pntCenter.y);
			stepper_ctrl(pntCur.x - pntPre.x, pntCur.y - pntPre.y);
			if ((int)angle != anglePre)// ghi gia tri len uart
			{
				//UART_printXY(pntCur.x, pntCur.y);
				//anglePre = (int)angle;
			}
			pntPre.x = pntCur.x;
			pntPre.y = pntCur.y;
			angle += deltaC;
		}
		gVal1.R = 0;
	}

	return pntEnd;
}
int main(void)
{

	struct gValue gVal;
	struct point pntStart;
	pntStart.x = pntStart.y = 0;

	stdout = &mystdout;

	UART_config(9600);
	stepper_config();
	UART_println("START!!!");
	sei();
	while(0)
	{
		for (int i = 0; i< 3200; i++)
		{
			stepper_ctrl(1, -1);
		}
		_delay_ms(1000);
		for (int i = 0; i< 3200; i++)
		{
			stepper_ctrl(-1, 1);
		}
		_delay_ms(1000);
	}
	while(1)
	{
		if (transmitted == truee)
		{
			//UART_println(uartBuffer);
			gVal = getAllGcodeValue(uartBuffer);
			//printf("G = %d\nX = %d\nY = %d\nR = %d\nI = %d\nJ = %d\n", (int)gVal.G, (int)gVal.X, (int)gVal.Y, (int)gVal.R, (int)gVal.I, (int)gVal.J);
			draw(gVal, pntStart);
			transmitted = falsee;
		}
		//UART_println("LOPP");
		_delay_ms(500);
	}
}
ISR(USART_RXC_vect)
{
	uartBuffer[indexUART] = UDR;
	if (transmitted == truee)
	{
		return;
	}
	if (uartBuffer[indexUART] == '@')
	{
		uartBuffer[indexUART] = '\0';
		indexUART = 0;
		transmitted = truee;
		return;
	}
	indexUART++;
}
