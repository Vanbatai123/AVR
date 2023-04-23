/*
 * DS1307.h
 *
 * Created: 12/24/2018 8:44:28 AM
 *  Author: Van_BasTai
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#define DS1307_ADDRESS		0x68

uint8_t bcd2dec(uint8_t bcd);
uint8_t dec2bcd(uint8_t dec);
void DS1307_read(uint8_t data[]);
void DS1307_write(uint8_t data[]);



#endif /* DS1307_H_ */