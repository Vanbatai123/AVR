#include <avr/interrupt.h>

#include "fifo.h"

#define INTR_DIS()	cli()
#define INTR_ENA()	sei()

typedef struct {
    uint8_t wp;
    uint8_t rp;
    uint16_t cnt;
    uint8_t data[256];
} fifo_t;

static fifo_t fifo;

void fifo_init(void)
{
    INTR_DIS();
    {
	fifo.wp = 0;
	fifo.rp = 0;
	fifo.cnt = 0;
    }
    INTR_ENA();
}

void fifo_read(uint8_t * c)
{
    INTR_DIS();
    {
	if (fifo.cnt > 0) {
	    *c = fifo.data[fifo.rp];
	    (fifo.rp)++;
	    (fifo.cnt)--;
	}
    }
    INTR_ENA();
}

void fifo_write(const uint8_t c)
{
    INTR_DIS();
    {
	if (fifo.cnt < sizeof(fifo.data)) {
	    fifo.data[fifo.wp] = c;
	    (fifo.wp)++;
	    (fifo.cnt)++;
	}
    }
    INTR_ENA();
}

bool fifo_full(void)
{
    bool r;

    INTR_DIS();
    {
	if (sizeof(fifo.data) <= fifo.cnt) {
	    r = true;
	} else {
	    r = false;
	}
    }
    INTR_ENA();

    return r;
}

bool fifo_empty(void)
{
    bool r;

    INTR_DIS();
    {
	if (fifo.cnt == 0) {
	    r = true;
	} else {
	    r = false;
	}
    }
    INTR_ENA();

    return r;
}

void fifo_count(uint16_t * cnt, uint16_t * size)
{
    INTR_DIS();
    {
	*cnt = fifo.cnt;
	*size = sizeof(fifo.data);
    }
    INTR_ENA();
}
