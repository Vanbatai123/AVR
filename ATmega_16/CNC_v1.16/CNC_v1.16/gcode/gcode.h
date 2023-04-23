/*
 * stepper.h
 *
 * Created: 10/26/2018 9:09:43 PM
 *  Author: PC
 */ 


#ifndef GCODE_H_
#define GCODE_H_

#define STEP_PER_MILI 100

struct gValue getAllGcodeValue(char *gCode);
int16_t getGcodeValue(char *gCode, char gValue);

#endif /* GCODE_H_ */