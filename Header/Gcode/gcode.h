/*
 * stepper.h
 *
 * Created: 10/26/2018 9:09:43 PM
 *  Author: PC
 */ 


#ifndef GCODE_H_
#define GCODE_H_

struct gValue getAllGcodeValue(char *gCode);
float getGcodeValue(char *gCode, char gValue);

#endif /* GCODE_H_ */