/*
 * USB_MOUSE.c
 *
 * Created: 1/7/2017 1:10:30 PM
 *  Author: Administrator
 */ 

//-----------------------------------------------------------------------------
// usb310.c
//                                                 2004.02.23  Kyosuke Ishikawa
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F:\AVR\Header\mySTART.h"
#include "F:\AVR\Header\myLCD_HP.h"
#include "F:\AVR\Header\myTIMER_1.h"
#include "F:\VI DIEU KHIEN\AVR\USB_MOUSE\usb310\usbhost.c"
//#include <c8051f310.h>				// SFR declarations


//-----------------------------------------------------------------------------
// Macro Definitions
//-----------------------------------------------------------------------------

/*
sbit D_P = P0^0;
sbit D_N = P0^1;*/
#define PORT_USB PORTB
#define DDR_USB DDRB
#define PIN_USB PINB
#define PORT_LED PORTA
#define DDR_LED DDRA
#define D_P 0
#define D_N 1
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

extern void config(void);

void uh_wai_1ms(void);
void uh_snd_eop(void);
void uh_snd_pkt(unsigned char, unsigned char);
void uh_snd_pkt_ack(unsigned char);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

unsigned char			osc_orig;

unsigned char idata		uh_state;
unsigned char			uh_time;

unsigned char idata		pkt_dat[UH_PACKET_LEN_MAX];
const unsigned char		pkt_ack_dat[20] = 
{
	UH_BUS_DATA_K, UH_BUS_DATA_J,
	UH_BUS_DATA_K, UH_BUS_DATA_J,
	UH_BUS_DATA_K, UH_BUS_DATA_J,
	UH_BUS_DATA_K, UH_BUS_DATA_K,
	UH_BUS_DATA_J, UH_BUS_DATA_J,
	UH_BUS_DATA_K, UH_BUS_DATA_J,
	UH_BUS_DATA_J, UH_BUS_DATA_K,
	UH_BUS_DATA_K, UH_BUS_DATA_K,
	UH_BUS_SE_ZERO, UH_BUS_SE_ZERO,
UH_BUS_IDLE, UH_BUS_IDLE };
unsigned char idata		rcv_dat[10];

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

void main(void)
{
	volatile unsigned short	loop;
	unsigned char			i;
	signed char				res;
	unsigned char			len;
	
	// Disable watchdog timer
	PCA0MD &= ~0x40;
	// Config internal register
	config();
	
	// Backup initial OSCICL and set new OSCICL value
	osc_orig = OSCICL;
	
	// Set initial port config and state
	DDR_USB = 0xFF;
	DDR_LED = 0xFF;	// LED OFF
	XBR1 = 0x40;
	P0MDOUT = 0x00;
	uh_state == UH_STATE_DISCONNECT;
	
	// Wait for connecting USB device
	#if 1
	do	{
		while( inbit(PIN_USB,D_P) == 1 && inbit(PIN_USB,D_N) == 1 );
		// 20ms wait
		for ( loop = 0 ; loop < 0xDFFF ; loop++ );
	} while( inbit(PIN_USB,D_P) == 1 && inbit(PIN_USB,D_N) == 1 );
	#endif

	PORT_LED = 0x00;							// LED ON
	EA = 1;

	// 1sec wait
	_delay_us(1000);

	// Detected USB device
	uh_state == UH_STATE_CONNECTED;

	// Reset state
	PORT_USB = UH_BUS_SE_ZERO;
	for ( loop = 0 ; loop < 0x4FFF ; loop++ );
	PORT_USB = UH_BUS_IDLE;

	// 40ms wait
	_delay_ms(40);
	
	// Get Descriptor ----------------

	// SETUP Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_SETUP, 0, 0);
	len = uh_cre_pkt_get_descriptor();
	uh_wai_1ms();
	uh_snd_pkt(len, 20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 20);

	// DATA Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_IN, 0, 0);
	uh_wai_1ms();
	uh_snd_pkt(len, 99);
	uh_snd_pkt_ack(20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 99);
	
	// STATUS Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_OUT, 0, 0);
	len = uh_cre_pkt_status_success();
	uh_wai_1ms();
	uh_snd_pkt(len, 20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 20);
	
	// Set Address ----------------

	// SETUP Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_SETUP, 0, 0);
	len = uh_cre_pkt_set_address(UH_ADDR);
	uh_wai_1ms();
	uh_snd_pkt(len, 20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 20);

	// STATUS Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_IN, 0, 0);
	uh_wai_1ms();
	uh_snd_pkt(len, 35);
	uh_snd_pkt_ack(20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 20);
	
	// Set Configuration ----------------

	// SETUP Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_SETUP, UH_ADDR, 0);
	len = uh_cre_pkt_set_configuration(UH_CONFIG_NO);
	uh_wai_1ms();
	uh_snd_pkt(len, 20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 20);

	// STATUS Transaction
	res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
	len = uh_cre_pkt_token(UH_PID_IN, UH_ADDR, 0);
	uh_wai_1ms();
	uh_snd_pkt(len, 35);
	uh_snd_pkt_ack(20);
	res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 35);
	
	P1 = 0xFF;		// LED OFF

	while(1)	{
		// Wait (INTERVAL - 1)ms
		for ( i=0 ; i<UH_DEFAULT_INTERVAL-1 ; i++ )	{
			uh_wai_1ms();
		}
		if ( uh_state == UH_STATE_CONNECTED )	{
			res = uh_cre_pkt_dat((unsigned char *)pkt_dat);
			len = uh_cre_pkt_token(UH_PID_IN, UH_ADDR, 1);
			uh_wai_1ms();
			uh_snd_pkt(len, 70);
			uh_snd_pkt_ack(20);
			res = uh_cnv_pkt_dat((unsigned char *)rcv_dat, (unsigned char *)pkt_dat, 70);
			if ( rcv_dat[1] != 0x5A )	{
				P1 = ~rcv_dat[2];
			}
		}
	}

	}	/* main */

	//-----------------------------------------------------------------------------

	void TIMER2_ISR(void) interrupt 5
	{
		if ( uh_state == UH_STATE_CONNECTED )	{
			uh_snd_eop();
		}
		uh_time++;
		TF2H = 0;
	}

	//-----------------------------------------------------------------------------

	void uh_wai_1ms(void)
	{
		unsigned char	last_time;
		
		last_time = uh_time;
		while(last_time == uh_time);
		
		return;
	}

	//-----------------------------------------------------------------------------

	void uh_snd_eop(void)
	{
		P0 = UH_BUS_SE_ZERO;
		P0 = UH_BUS_SE_ZERO;
		P0 = UH_BUS_IDLE;
	}

	//-----------------------------------------------------------------------------

	void uh_snd_pkt(unsigned char snd_len, unsigned char rcv_len)
	{
		unsigned char	i;

		// Packet send
		P0MDOUT = 0x03;
		OSCICL = osc_orig - 4;
		for ( i=0 ; i<snd_len ; i++ )	{
			P0 = pkt_dat[i];
		}
		P0MDOUT = 0x00;
		while( D_N == 1 && TF2H == 0 );
		OSCICL = osc_orig + 13;
		// Packet receive
		for ( i=0 ; i<rcv_len ; i++ )	{
			pkt_dat[i] = P0;
		}
		OSCICL = osc_orig;
	}

	//-----------------------------------------------------------------------------

	void uh_snd_pkt_ack(unsigned char snd_len)
	{
		unsigned char	i;

		// ACK Packet send
		P0MDOUT = 0x03;
		OSCICL = osc_orig - 4;
		for ( i=0 ; i<snd_len ; i++ )	{
			P0 = pkt_ack_dat[i];
		}
		OSCICL = osc_orig;
		P0MDOUT = 0x00;
	}

	//-----------------------------------------------------------------------------
