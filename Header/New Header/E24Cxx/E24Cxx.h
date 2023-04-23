/*
 * _24Cxx.h
 *
 * Created: 12/24/2018 1:21:49 PM
 *  Author: Van_BasTai
 */ 


#ifndef E24CXX_H_
#define E24CXX_H_

#define E24CXX_ADDRESS	0x57

void E24C_config();
void E24C_write(unsigned int addr, uint8_t data);
uint8_t E24C_read(unsigned int addr);



#endif /* 24CXX_H_ */