#ifndef _USART_H_
#define _USART_H_

void usart_init(void (*p)(const char c));
void usart_tx(char);

#endif

