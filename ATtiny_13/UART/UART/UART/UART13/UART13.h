/*
 * UART13.h
 *
 * Created: 4/10/2018 8:14:58 AM
 *  Author: Van_BasTai
 */ 


#ifndef UART13_H_
#define UART13_H_

#define SOFT_TX_PIN (1<<PB1)
#define SOFT_TX_PORT PORTB
#define SOFT_TX_DDR DDRB

void uart_tx_init ();
void num_to_str(unsigned int value, unsigned char nDigit);
void uart_print(char *str);
void uart_send_byte (unsigned char data);



#endif /* UART13_H_ */