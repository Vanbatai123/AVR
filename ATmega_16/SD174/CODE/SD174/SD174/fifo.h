#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdbool.h>
#include <inttypes.h>

void fifo_init(void);
void fifo_read(uint8_t * c);
void fifo_write(const uint8_t c);
bool fifo_full(void);
bool fifo_empty(void);
void fifo_count(uint16_t *cnt, uint16_t *size);

#endif
