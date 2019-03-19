// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <LedControl.h>

extern void InitialDisplay();
extern void ClearDisplay();
extern void DisplayBlinkVariable(char* str, int length);
extern void DisplayTemperatureHumidity(float temperature, float humidity);
extern void DisplayTemperature(float temperature);
extern void DisplayHumidity(float humidity);
extern void DisplayTimer(unsigned int timer);
extern void DisplayRollingTime(unsigned int rollingTime);
extern void DisplayStartStop(bool startStop);
extern void DisplayUpdateVal();
#endif

