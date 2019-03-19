// Buttons.h

#ifndef _BUTTONS_h
#define _BUTTONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <TimerOne.h>

extern void InitializeButtonsPins();
extern void RegulateFanAndBulb(float* temperature);
extern void HandleCurrentInterruptMode();
void HandleTemperatureSet();
#endif

