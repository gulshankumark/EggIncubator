// TempHumidity.h

#ifndef _TEMPHUMIDITY_h
#define _TEMPHUMIDITY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <SimpleDHT.h>
extern int ReadTemperatureHumidity(float* temperature, float* humidity, int* err);
#endif

