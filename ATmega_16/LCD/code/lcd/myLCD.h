#include <util/delay.h>
#define sbi(sfr,bit) sfr|=_BV(bit)
#define cbi(sfr,bit) sfr&=~(_BV(bit))
#define EN                            2
#define RW                           1
#define RS                            0
#define CTRL                       PORTB
#define DDR_CTRL             DDRB
#define DATA_O                  PORTB
#define DATA_I                    PINB
#define DDR_DATA             DDRB
/*
#define LCD8BIT
#define DATA_O                  PORTD
#define DATA_I                    PIND
#define DDR_DATA             DDRD
*/
