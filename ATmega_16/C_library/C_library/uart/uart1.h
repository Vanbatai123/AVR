/*
* UART.h
*
* Created: 10/27/2018 10:23:05 AM
*  Author: PC
*/


#ifndef UART_H_
#define UART_H_

//#define USE_PRINTF

#define BIN		2
#define DEC		10
#define HEX		16

#ifdef USE_PRINTF
int usart_putchar_printf(char var, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
stdout = &mystdout;
#endif

void UART_config(uint32_t baudrate);
void UART_putChar(char c);
void UART_print(char* str);
void UART_printNum(long num, uint8_t base);
void UART_printNumln(long num, uint8_t base);
void UART_printFloat(double num, uint8_t digits);
void UART_printFloatln(double num, uint8_t digits);
void UART_println();
uint8_t UART_getChar();


#endif /* UART_H_ */