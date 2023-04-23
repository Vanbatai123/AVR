#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>
#include <inttypes.h>
#include "diskio.h"
#include "pff.h"
#include "xitoa.h"
#include "usart.h"
#include "fifo.h"

#define LED_ON()    PORTD |= (1 << 3)
#define LED_OFF()   PORTD &= ~(1 << 3)

#define FCC(c1,c2,c3,c4)	(((DWORD)c4<<24)+((DWORD)c3<<16)+((WORD)c2<<8)+(BYTE)c1)

enum Request {
    RequestStop,
    RequestPlay
};

enum State {
    StateStop,
    StatePreload,
    StatePlay
};

#define TMR0SPD_MIN (-40)
#define TMR0SPD_MAX (+20)

typedef struct {
    uint8_t channels;
    uint8_t bits;
    uint16_t freq;
} wavinfo_t;

typedef struct {
    enum Request request;
    enum State state;
    uint8_t tmr0val;
    int8_t tmr0err;
    int8_t tmr0spd;
    wavinfo_t wavinfo;
} control_t;

control_t control;

FATFS fs;			/* File system object */
DIR dir;			/* Directory object */
FILINFO fno;			/* File information */

void timer0_init(void);
void timer0_fini(void);
void timer1_init(void);
void timer1_fini(void);
void timer2_init(void);
void timer2_fini(void);
DWORD load_header(control_t * c);
UINT play(const char *fn, control_t * c);
void hardware_init(void);
void user_command(const char c);
int main(void);

/**
 * TIMER0を初期化する.
 */
void timer0_init(void)
{
    TCCR0 = 0x02;
    TIMSK |= _BV(TOIE0);
}

void timer0_fini(void)
{
    TIMSK &= ~_BV(TOIE0);
}

/**
 * TIMER0割り込みサービスを実行する.
 */
ISR(TIMER0_OVF_vect)
{
    TCNT0 = control.tmr0val + control.tmr0err + control.tmr0spd;

    /*
     * データサイズ確認用に取得しておく.
     */
    uint16_t cnt = 0;
    uint16_t size = 0;
    fifo_count(&cnt, &size);

    /*
     * FIFOサイズの半分以上のデータが蓄積されたら再生状態にする.
     */
    if (StatePlay != control.state) {
	if (((size >> 1) < cnt) && (StatePreload == control.state)) {
	    control.state = StatePlay;
	}
	return;
    }

    if (16 == control.wavinfo.bits) {
	if (cnt < 4) {
	    return;
	}

	/*
	 * 16bits mono and stereo
	 */
	uint8_t datLH = 0, datLL = 0;
	uint8_t datRH = 0, datRL = 0;

	fifo_read(&datLL);
	fifo_read(&datLH);
	if (2 == control.wavinfo.channels) {
	    fifo_read(&datRL);
	    fifo_read(&datRH);
	}

	/*
	 * signedをオフセットのついたunsignedにするには
	 * 最上位の符号ビットを反転するだけで良いのだ.
	 */
	const uint16_t dl = (datLH << 8 | datLL) ^ 0x8000;

	/*
	 * 8bit fast pwmなのでビットシフトしてデータ階調を落とす.
	 */
	OCR1A = dl >> 8;
    } else if (8 == control.wavinfo.bits) {
	if (cnt < 2) {
	    return;
	}

	/*
	 * 8bits mono and stereo
	 */
	uint8_t datL, datR;
	fifo_read(&datL);
	if (2 == control.wavinfo.channels) {
	    fifo_read(&datR);
	}

	/*
	 * 8bit fast pwmなのでビットシフトの必要はない.
	 */
	OCR1A = datL;
    }
}

/*
 * ===============================================
 * TIMER1.
 * ===============================================
 */
/**
 * TIMER1を初期化する.
 */
void timer1_init(void)
{
    TCCR1A =
	(1 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0
									 <<
									 WGM11)
	| (1 << WGM10);
    TCCR1B =
	(0 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (1 <<
								   CS10);
}

void timer1_fini(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
}

void timer2_init(void)
{
    TCCR2 = (0 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK |= _BV(TOIE2);
}

void timer2_fini(void)
{
    TCCR2 = 0;
    TIMSK &= ~_BV(TOIE2);
}

ISR(TIMER2_OVF_vect)
{
    /*
     * スイッチ入力検出.
     */
    static uint8_t swprev = 0x00;
    uint8_t swcurr = PINB & 0x01;
    if (0 != (swprev ^ swcurr)) {
	if (0 == (swcurr & 0x01)) {
	    if (RequestStop == control.request) {
		xprintf(PSTR("PLAY\r\n"));
		control.request = RequestPlay;
	    } else {
		xprintf(PSTR("STOP\r\n"));
		control.request = RequestStop;
	    }
	}
    }
    swprev = swcurr;
}

/*---------------------------------------------------------*/

/* 0:Invalid format, 1:I/O error, >1:Number of samples */
DWORD load_header(control_t * c)
{
    WORD rb;
    DWORD fcc, sz;
    UINT i;
    FRESULT res;
    BYTE Buff[256];

    res = pf_read(Buff, 256, &rb);
    if (res) {
	return 1;
    }

    if (rb != 256 || (LD_DWORD(Buff + 8) != FCC('W', 'A', 'V', 'E'))) {
	return 0;
    }

    i = 12;
    while (i < 200) {
	fcc = LD_DWORD(&Buff[i]);	/* FCC */
	sz = LD_DWORD(&Buff[i + 4]);	/* Chunk size */
	i += 8;
	switch (fcc) {

	case FCC('f', 'm', 't', ' '):	/* 'fmt ' chunk */
	    if (sz > 100 || sz < 16) {
		/* Check chunk size */
		return 0;
	    }
	    if (Buff[i + 0] != 1) {
		/* Check coding type (1) */
		return 0;
	    }
	    if (Buff[i + 2] != 1 && Buff[i + 2] != 2) {
		/* Check channels (1/2) */
		return 0;
	    }
	    if (Buff[i + 14] != 8 && Buff[i + 14] != 16) {
		/* Check resolution (8/16) */
		return 0;
	    }
	    c->wavinfo.channels = Buff[i + 2];
	    c->wavinfo.bits = Buff[i + 14];
	    c->wavinfo.freq = LD_WORD(&Buff[i + 4]);
	    c->tmr0val = 255 - (F_CPU / 8 / LD_WORD(&Buff[i + 4]));	/* Sampling freq */
	    break;

	case FCC('f', 'a', 'c', 't'):	/* 'fact' chunk (skip) */
	    break;

	case FCC('d', 'a', 't', 'a'):	/* 'data' chunk (start to play) */
	    fs.fptr = i;
	    return sz;

	default:		/* Unknown chunk (error) */
	    return 0;
	}
	i += sz;
    }

    return 0;
}

UINT play(const char *fn, control_t * c)
{
    WORD rb;
    DWORD sz;
    FRESULT res;
    WORD btr;

    if ((res = pf_open(fn)) == FR_OK) {

	sz = load_header(c);
	if (sz < 256) {
	    return (UINT) sz;
	}

	xprintf(PSTR("%s : "), fn);
	xprintf(PSTR("Ch:%u, Bits:%u, Freq:%u\r\n"),
		c->wavinfo.channels, c->wavinfo.bits, c->wavinfo.freq);

	c->state = StatePreload;
	pf_read(0, 512 - fs.fptr, &rb);	/* Snip sector unaligned part */
	sz -= rb;
	do {
	    /*
	     * 停止状態をリクエストされたら内部状態を停止にする.
	     */
	    bool stop = false;
	    while (RequestStop == c->request) {
		c->state = StateStop;
		stop = true;
	    }
	    if (stop) {
		c->state = StatePreload;
	    }

	    /*
	     * データを転送する.
	     */
	    btr = (sz > 1024) ? 1024 : (WORD) sz;
	    res = pf_read(0, btr, &rb);
	    if (res != FR_OK || btr != rb) {
		break;
	    }
	    sz -= rb;
	} while (rb == 1024);
    }
    c->state = StateStop;

    return res;
}


void hardware_init(void)
{
    PORTB = 0xEF;
    DDRB = 0xEE;

    PORTC = 0xFF;
    DDRC = 0x00;

    PORTD = 0xFC;
    DDRD = 0x0A;
}

void user_command(const char c)
{
    switch (c) {
    case 'p':
	xprintf(PSTR("PLAY\r\n"));
	control.request = RequestPlay;
	break;
    case 's':
	xprintf(PSTR("STOP\r\n"));
	control.request = RequestStop;
	break;
    case '+':
	if (control.tmr0spd < TMR0SPD_MAX) {
	    xprintf(PSTR("SPEED=%d\r\n"), ++control.tmr0spd);
	}
	break;
    case '-':
	if (TMR0SPD_MIN < control.tmr0spd) {
	    xprintf(PSTR("SPEED=%d\r\n"), --control.tmr0spd);
	}
	break;
    case '0':
	control.tmr0spd = 0;
	xprintf(PSTR("SPEED=%d\r\n"), control.tmr0spd);
	break;
#if 0
    case ' ':
    case 0x0D:
	if (RequestStop == control.request) {
	    xprintf(PSTR("PLAY\r\n"));
	    control.request = RequestPlay;
	} else {
	    xprintf(PSTR("STOP\r\n"));
	    control.request = RequestStop;
	}
	break;
    case 'a':
	xprintf(PSTR("OSCCAL=%d\r\n"), ++OSCCAL);
	break;
    case 'z':
	xprintf(PSTR("OSCCAL=%d\r\n"), --OSCCAL);
	break;
    case 'h':
	xprintf(PSTR("[Functions]\r\n"));
	xprintf(PSTR(" h:Help\r\n"));
	xprintf(PSTR(" p:Play\r\n"));
	xprintf(PSTR(" s:Stop\r\n"));
	xprintf(PSTR(" +:Speed up\r\n"));
	xprintf(PSTR(" -:Speed down\r\n"));
	xprintf(PSTR(" 0:Speed reset\r\n"));
	xprintf(PSTR(" a:Clock up\r\n"));
	xprintf(PSTR(" z:Clock down\r\n"));
	break;
#endif
    default:
	xprintf(PSTR("?\r\n"));
	break;
    }
}

int main(void)
{

    /*
     * B041はRC内蔵クロックを使っている.
     * デバイスによって個体差があるので調整を必要とするだろう.
     */
    OSCCAL = 180;

    cli();

    hardware_init();

    xfunc_out = usart_tx;
    usart_init(user_command);

    timer0_init();
    timer1_init();
    timer2_init();

    fifo_init();

    control.request = RequestStop;
    control.state = StateStop;
    control.tmr0val = 255 - 100;
    control.tmr0err = 10;
    control.tmr0spd = 0;

    sei();

    for (;;) {
	if (pf_mount(&fs)) {
	    continue;
	}
	for (;;) {
	    if (pf_opendir(&dir, "")) {
		break;
	    }
	    while (!pf_readdir(&dir, &fno) && fno.fname[0]) {
		if (!(fno.fattrib & (AM_DIR | AM_HID))
		    && strstr(fno.fname, ".WAV")) {
		    if (play(fno.fname, &control)) {
			break;
		    }
		}
	    }
	}
    }
}
