// RTCLogic.h

#ifndef _RTCLogic_h
#define _RTCLogic_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <RTClib.h>

//char DaysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
extern void InitializeRTC();
extern void DoMotorOperation(unsigned long* startMillis, unsigned long* currentMillis);
extern DateTime GetCurrentDateTime();
extern void SetTime();
#endif

