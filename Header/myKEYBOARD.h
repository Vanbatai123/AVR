#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <stdio.h>

#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) (thanhghi=thanhghi^(1<<bit))

#include "usbdrv.h"
#include "oddebug.h"



/* ----------------------- hardware I/O abstraction ------------------------ */

/* pin assignments:
PB0	Key 1
PB1	Key 2
PB2	Key 3
PB3	Key 4
PB4	Key 5
PB5 Key 6

PC0	Key 7
PC1	Key 8
PC2	Key 9
PC3	Key 10
PC4	Key 11
PC5	Key 12

PD0	USB-
PD1	debug tx
PD2	USB+ (int0)
PD3	Key 13
PD4	Key 14
PD5	Key 15
PD6	Key 16
PD7	Key 17
*/


static void hardwareInit(void)//x
{
	uchar	i, j;
// 	clrb(USB_PORT,USB_CFG_DMINUS_BIT);
// 	clrb(USB_PORT,USB_CFG_DPLUS_BIT);
// 	PORTD = 0xf9;   /* 1111 1001 bin: activate pull-ups except on USB lines */
 	USB_PORT = (0<<USB_CFG_DMINUS_BIT)|(0<<USB_CFG_DPLUS_BIT);
	 
// 	setb(USB_DDR,USB_CFG_DMINUS_BIT);
// 	setb(USB_DDR,USB_CFG_DPLUS_BIT);
//  DDRD = 0x06;    /* 0000 0110 bin: all pins input except USB (-> USB reset) */
	USB_DDR = (1<<USB_CFG_DMINUS_BIT)|(1<<USB_CFG_DPLUS_BIT);
	
	j = 0;
	while(--j)/* USB Reset by device only required on Watchdog Reset */
	{     
		i = 0;
		while(--i); /* delay >10ms for USB reset */
	}
// 	clrb(USB_DDR,USB_CFG_DMINUS_BIT);
// 	clrb(USB_DDR,USB_CFG_DPLUS_BIT);
//  DDRD = 0x01;    /* 0000 0010 bin: remove USB reset condition */
					/* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
	USB_DDR = (0<<USB_CFG_DMINUS_BIT)|(0<<USB_CFG_DPLUS_BIT);
	
    TCCR0 = 5;      /* timer 0 prescaler: 1024 */
}
/* ------------------------------------------------------------------------- */
/* The following function returns an index for the first key pressed. It
 * returns 0 if no key is pressed.
 */
static uchar keyPressed()
{
	if (inbit(PINB,0)==0)
		return 13;
	else if (inbit(PINB,1)==0)
		return 14;
	else if (inbit(PINB,2)==0)
		return 15;
	else
		return 0;
}
/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
static uchar    reportBuffer[2];    /* buffer for HID reports */
static uchar    idleRate;           /* in 4 ms units */

const PROGMEM char usbHidReportDescriptor[35] =	{   /* USB report descriptor */
													0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
													0x09, 0x06,                    // USAGE (Keyboard)
													0xa1, 0x01,                    // COLLECTION (Application)
													0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
													0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
													0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
													0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
													0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
													0x75, 0x01,                    //   REPORT_SIZE (1)
													0x95, 0x08,                    //   REPORT_COUNT (8)
													0x81, 0x02,                    //   INPUT (Data,Var,Abs)
													0x95, 0x01,                    //   REPORT_COUNT (1)
													0x75, 0x08,                    //   REPORT_SIZE (8)
													0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
													0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
													0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
													0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
													0xc0                           // END_COLLECTION
												};
/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and we only allow one
 * simultaneous key press (except modifiers). We can therefore use short
 * 2 byte input reports.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */

/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
//----------------------------------------------------------------------//
//#define
#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)
//----------------------------------------------------------------------//
#define KEY_NON					0
// #define KEY_					1
// #define KEY_					2
// #define KEY_					3
#define KEY_A       			4
#define KEY_B       			5
#define KEY_C       			6
#define KEY_D       			7
#define KEY_E       			8
#define KEY_F       			9
#define KEY_G       			10
#define KEY_H       			11
#define KEY_I       			12
#define KEY_J       			13
#define KEY_K       			14
#define KEY_L       			15
#define KEY_M       			16
#define KEY_N       			17
#define KEY_O       			18
#define KEY_P       			19
#define KEY_Q       			20
#define KEY_R       			21
#define KEY_S       			22
#define KEY_T       			23
#define KEY_U       			24
#define KEY_V       			25
#define KEY_W       			26
#define KEY_X       			27
#define KEY_Y       			28
#define KEY_Z       			29
#define KEY_1       			30
#define KEY_2       			31
#define KEY_3       			32
#define KEY_4       			33
#define KEY_5       			34
#define KEY_6       			35
#define KEY_7       			36
#define KEY_8       			37
#define KEY_9       			38
#define KEY_0       			39

#define KEY_ENTER				40
// #define KEY_					41
#define KEY_BACK_SPACE			42
#define KEY_TAB					43
#define KEY_SPACE				44	/*   */	
#define KEY_MINUS				45	/* - */
#define KEY_EQUAL				46	/* = */
#define KEY_SQUA_BRACKET_OPEN	47	/* [ */
#define KEY_SQUA_BRACKET_CLOSE	48	/* ] */
#define KEY_BACK_SLASH_OPEN_1	49	/* \ */
#define KEY_BACK_SLASH_OPEN_2	50	/* \ */
#define KEY_SEMICOLON				51	/* ; */
#define KEY_APOS				52	/* ' */
#define KEY_NOT					53	/* ` */
#define KEY_COMMA				54
#define KEY_DOT					55	/* . */
#define KEY_BACK_SLASH_CLOSE	56	/* / */
#define KEY_CAPS_LOCK			57	/* [ */

#define KEY_F1      			58
#define KEY_F2      			59
#define KEY_F3      			60
#define KEY_F4      			61
#define KEY_F5      			62
#define KEY_F6      			63
#define KEY_F7      			64
#define KEY_F8      			65
#define KEY_F9      			66
#define KEY_F10     			67
#define KEY_F11     			68
#define KEY_F12     			69
// #define KEY_					70

#define KEY_SCROLL_LOCK			71
//#define KEY_					72
#define KEY_INSERT				73
#define KEY_HOME				74
#define KEY_PAGE_UP				75
#define KEY_DELETE				76
#define KEY_END					77
#define KEY_PAGE_DOWN			78
#define KEY_RIGHT				79
#define KEY_LEFT				80
#define KEY_DOWN				81
#define KEY_UP					82
#define KEY_NUM_LOCK			83
// #define KEY_					84
#define KEY_STAR				85	/* * */
#define KEY_MINUS_2				86	/* - */
#define KEY_PLUS				87	/* + */
#define KEY_ENTER_2				88
#define KEY_END_2				89
//#define KEY_					90
#define KEY_PAGE_DOWN_2			91
#define KEY_LEFT_2				92
#define KEY_RIGHT_2				94
#define KEY_HOME_2				95
//#define KEY_					96
#define KEY_PAGE_UP_2			97
#define KEY_INSERT_2			98
#define KEY_DELETE_2			99

#define KEY_BACK_SLASH_OPEN_3	100	/* \ */
#define KEY_BETWEEN_ALT_CTRL	101
//#define KEY_					101

#define NUM_KEYS    17
//----------------------------------------------------------------------//
static const uchar  keyReport[NUM_KEYS + 1][2] PROGMEM = {
	/* none */  {0, 0},
	/*  1 */    {MOD_SHIFT_LEFT, KEY_A},
	/*  2 */    {MOD_SHIFT_LEFT, KEY_B},
	/*  3 */    {MOD_SHIFT_LEFT, KEY_C},
	/*  4 */    {MOD_SHIFT_LEFT, KEY_D},
	/*  5 */    {MOD_SHIFT_LEFT, KEY_E},
	/*  6 */    {MOD_SHIFT_LEFT, KEY_F},
	/*  7 */    {MOD_SHIFT_LEFT, KEY_G},
	/*  8 */    {MOD_SHIFT_LEFT, KEY_H},
	/*  9 */    {MOD_SHIFT_LEFT, KEY_I},
	/* 10 */    {MOD_SHIFT_LEFT, KEY_J},
	/* 11 */    {MOD_SHIFT_LEFT, KEY_K},
	/* 12 */    {MOD_SHIFT_LEFT, KEY_L},
	/* 13 */    {MOD_SHIFT_LEFT, KEY_M},
	/* 14 */    {MOD_SHIFT_LEFT, KEY_N},
	/* 15 */    {MOD_SHIFT_LEFT, KEY_O},
	/* 16 */    {MOD_SHIFT_LEFT, KEY_P},
	/* 17 */    {MOD_SHIFT_LEFT, KEY_Q},
};
//----------------------------------------------------------------------//
static void buildReport(uchar key)/* This (not so elegant) cast saves us 10 bytes of program memory */
{
	*(int *)reportBuffer = pgm_read_word(keyReport[key]);
}
//----------------------------------------------------------------------//
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	
    usbMsgPtr = reportBuffer;
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)/* class request type */
	{    
        if(rq->bRequest == USBRQ_HID_GET_REPORT)	/* wValue: ReportType (highbyte), ReportID (lowbyte) */
													/* we only have one report type, so don't look at wValue */
		{  
			buildReport(keyPressed());
			return sizeof(reportBuffer);
        }
		else if(rq->bRequest == USBRQ_HID_GET_IDLE)
		{
            usbMsgPtr = &idleRate;
            return 1;
        }
		else if(rq->bRequest == USBRQ_HID_SET_IDLE)
		{
            idleRate = rq->wValue.bytes[1];
        }
    }
	else
	{
        /* no vendor specific requests implemented */
    }
	return 0;
}
/* ------------------------------------------------------------------------- */
void KEYBOARD_Init()
{
	wdt_enable(WDTO_2S);
	hardwareInit();
	odDebugInit();
	usbInit();
	sei();
	DBG1(0x00, 0, 0);
}
void EXAMPLE()
{
	char key;
	int code[6]={100,101,102,103,104,105};
	wdt_reset();
	usbPoll();
	// 		key = keyPressed();
	if (inbit(PINB,0)==0)
		key = code[0];
	else if (inbit(PINB,1)==0)
		key = code[1];
	else if (inbit(PINB,2)==0)
		key = code[2];
	else if (inbit(PINB,3)==0)
		key = code[3];
	else if (inbit(PINB,4)==0)
		key = code[4];
	else if (inbit(PINB,5)==0)
		key = code[5];
	else
		key = 0;
	if(usbInterruptIsReady())
	{
		if (key == 0)
		{
			reportBuffer[0] = reportBuffer[1] = 0;
		}
		else
		{
			reportBuffer[0] = MOD_SHIFT_LEFT;
			reportBuffer[1] = key;
		}
		
		usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
	}
}
void build_report_from_char(char c)
{
	if (c >= '0' && c <= '9')
	{
		reportBuffer[0] = 0;
		reportBuffer[1] = KEY_0 + c + '0';
	}
	else if (c >= 'a' && c <= 'z')
	{
		reportBuffer[0] = 0;
		reportBuffer[1] = KEY_A + c - 'a';
	}
	else if (c >= 'A' && c <= 'Z')
	{
		reportBuffer[0] = MOD_SHIFT_LEFT;
		reportBuffer[1] = KEY_A + c - 'A';
	}
	else
	{
		switch (c)
		{
			case '\n':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_ENTER;
			break;

			case ' ':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_SPACE;
			break;

			case '-':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_MINUS;
			break;
			case '_':
			reportBuffer[0] = MOD_SHIFT_LEFT;
			reportBuffer[1] = KEY_MINUS;
			break;

			case '=':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_EQUAL;
			break;
			case '+':
			reportBuffer[0] = MOD_SHIFT_LEFT;
			reportBuffer[1] = KEY_EQUAL;
			break;

			case ';':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_SEMICOLON;
			break;
			case ':':
			reportBuffer[0] = MOD_SHIFT_LEFT;
			reportBuffer[1] = KEY_SEMICOLON;
			break;

			case ',':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_COMMA;
			break;
			case '<':
			reportBuffer[0] = MOD_SHIFT_LEFT;
			reportBuffer[1] = KEY_COMMA;
			break;

			case '.':
			reportBuffer[0] = 0;
			reportBuffer[1] = KEY_DOT;
			break;
			case '>':
			reportBuffer[0] = MOD_SHIFT_LEFT;
			reportBuffer[1] = KEY_DOT;
			break;

			default:
			reportBuffer[0] = 0;
			reportBuffer[1] = 0;
		}
	}
}
int pointer = 0, send_enable = 0;
void KEYBOARD_print(char* text)
{
	wdt_reset();
	usbPoll();	
	if (inbit(PINB,1)==0)
		send_enable = 1;
	if(send_enable && usbInterruptIsReady())
	{
		build_report_from_char(text[pointer]);
		send_enable = text[pointer++];
		if (send_enable == 0)
		{
			pointer = 0;
			_delay_ms(300);
		}
		usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
	}
}