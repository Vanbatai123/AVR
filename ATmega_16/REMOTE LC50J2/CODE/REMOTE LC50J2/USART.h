#ifndef __USART_H
#define __USART_H

#include "StdTypes.h"

void USART_Init(void);
void USART_Transmit(uint8_t data);
unsigned char USART_Receive( void );

#endif /*__USART_H*/
