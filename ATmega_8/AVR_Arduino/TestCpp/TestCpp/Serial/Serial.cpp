/*
 * Serial.cpp
 *
 * Created: 7/26/2018 8:50:15 PM
 *  Author: ASUS PC
 */ 

#include "../include.h"
#include "Serial.h"

// Public Methods //////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial()
{
	
}

void HardwareSerial::begin(unsigned long baud, byte config)
{
	UBRRL = F_CPU/(16*baud) - 1; // calculate UBBR
	UBRRH = 0;
	
	UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
	UCSRB |= (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
}
/*
void HardwareSerial::end()
{
	// wait for transmission of outgoing data
	flush();

	UCSRB &= (~(1<<RXEN)) & (~(1<<TXEN)) & (~(1<<RXCIE)) & (~(1<<UDRIE));
	
	// clear any received data
	_rx_buffer_head = _rx_buffer_tail;
}

int HardwareSerial::available(void)
{
	return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

int HardwareSerial::peek(void)
{
	if (_rx_buffer_head == _rx_buffer_tail) {
		return -1;
		} else {
		return _rx_buffer[_rx_buffer_tail];
	}
}

int HardwareSerial::read(void)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (_rx_buffer_head == _rx_buffer_tail) {
		return -1;
		} else {
		unsigned char c = _rx_buffer[_rx_buffer_tail];
		_rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
		return c;
	}
}

int HardwareSerial::availableForWrite(void)
{
	#if (SERIAL_TX_BUFFER_SIZE>256)
	uint8_t oldSREG = SREG;
	cli();
	#endif
	tx_buffer_index_t head = _tx_buffer_head;
	tx_buffer_index_t tail = _tx_buffer_tail;
	#if (SERIAL_TX_BUFFER_SIZE>256)
	SREG = oldSREG;
	#endif
	if (head >= tail) return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
	return tail - head - 1;
}

void HardwareSerial::flush()
{
	// If we have never written a byte, no need to flush. This special
	// case is needed since there is no way to force the TXC (transmit
	// complete) bit to 1 during initialization
	if (!_written)
	return;

	while (bit_is_set(*_ucsrb, UDRIE0) || bit_is_clear(*_ucsra, TXC0)) {
		if (bit_is_clear(SREG, SREG_I) && bit_is_set(*_ucsrb, UDRIE0))
		// Interrupts are globally disabled, but the DR empty
		// interrupt should be enabled, so poll the DR empty flag to
		// prevent deadlock
		if (bit_is_set(*_ucsra, UDRE0))
		_tx_udr_empty_irq();
	}
	// If we get here, nothing is queued anymore (DRIE is disabled) and
	// the hardware finished tranmission (TXC is set).
}
*/
size_t HardwareSerial::write(uint8_t c)
{
	while (inbit(UCSRA, UDRE) == 0); //cho den khi bit UDRE=1 Neu = 1 thi viet code vao
	UDR = c;
	return 1;
}


size_t HardwareSerial::print(char c)
{
	return write(c);
}

size_t HardwareSerial::print(const char *str)
{
	for (uint8_t i = 0; str[i] != 0; i++)	print(str[i]);
	return 0;
}

size_t HardwareSerial::print(unsigned char b, int base)
{
	return print((unsigned long) b, base);
}

size_t HardwareSerial::print(int n, int base)
{
	return print((long) n, base);
}

size_t HardwareSerial::print(unsigned int n, int base)
{
	return print((unsigned long) n, base);
}

size_t HardwareSerial::print(long n, int base)
{
	if (base == 0) {
		return write(n);
		} else if (base == 10) {
		if (n < 0) {
			int t = print('-');
			n = -n;
			return printNumber(n, 10) + t;
		}
		return printNumber(n, 10);
		} else {
		return printNumber(n, base);
	}
}

size_t HardwareSerial::print(unsigned long n, int base)
{
	if (base == 0) return write(n);
	else return printNumber(n, base);
}

size_t HardwareSerial::print(double n, int digits)
{
	return printFloat(n, digits);
}



size_t HardwareSerial::println(const char *str)
{
	print(str);
	return println();
}
size_t HardwareSerial::println(char c)
{
	print(c);
	return println();
}
size_t HardwareSerial::println(unsigned char b, int base)
{
	print(b, base);
	return println();
}
size_t HardwareSerial::println(int n, int base)
{
	print(n, base);
	return println();
}
size_t HardwareSerial::println(unsigned int n, int base)
{
	print(n, base);
	return println();
}
size_t HardwareSerial::println(long n, int base)
{
	print(n, base);
	return println();
}
size_t HardwareSerial::println(unsigned long n, int base)
{
	print(n, base);
	return println();
}
size_t HardwareSerial::println(double n, int digit)
{
	print(n, digit);
	return println();
}
size_t HardwareSerial::println(void)
{
	return print("\r\n");
}

// Private Methods /////////////////////////////////////////////////////////////
size_t HardwareSerial::printNumber(unsigned long n, uint8_t base)
{
	char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	if (base < 2) base = 10;

	do {
		char c = n % base;
		n /= base;

		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n);

	return print(str);
}

size_t HardwareSerial::printFloat(double number, uint8_t digits)
{
	size_t n = 0;
	
	if (isnan(number)) return print("nan");
	if (isinf(number)) return print("inf");
	if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
	if (number <-4294967040.0) return print ("ovf");  // constant determined empirically
	
	// Handle negative numbers
	if (number < 0.0)
	{
		n += print('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;
	
	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	n += print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		n += print('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		n += print(toPrint);
		remainder -= toPrint;
	}
	
	return n;
}
