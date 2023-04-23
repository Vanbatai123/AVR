/*
* stepper.c
*
* Created: 10/26/2018 9:09:27 PM
*  Author: PC
*/
#include "../include.h"
#include "gcode.h"

struct gValue getAllGcodeValue(char *gCode)
{
	struct gValue gval1;
	
	gval1.G = getGcodeValue(gCode, 'G');
	gval1.X = getGcodeValue(gCode, 'X');
	gval1.Y = getGcodeValue(gCode, 'Y');
	gval1.Z = getGcodeValue(gCode, 'Z');
	gval1.I = getGcodeValue(gCode, 'I');
	gval1.J = getGcodeValue(gCode, 'J');
	
	return gval1;
}


float getGcodeValue(char *gCode, char gValue)
{
	char *gTemp;
	gTemp = malloc(7);
	uint8_t j = 0, exists = falsee;
	
	for (uint8_t i = 0; gCode[i]!= '\0'; i++){
		if (exists == truee){
			if (gCode[i] != ' '){
				gTemp[j++] = gCode[i];
			}
			else{
				gTemp[j] = '\0';
				break;
			}
		}
		else if (gCode[i] == gValue){
			exists = truee;
		}
	}
	
	float reValue = atof(gTemp);
	free(gTemp);
	return reValue;
}