// GlobalConstantsVariables.h

#ifndef _GLOBALCONSTANTSVARIABLES_h
#define _GLOBALCONSTANTSVARIABLES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DEBUG 0

#define TEMP_HUMIDITY_PIN 12
#define I2C_SDA 8
#define I2C_SCL 9
#define DISPLAY_DIN A2
#define	DISPLAY_CS A0
#define DISPLAY_CLK A1
#define SET_BUTTON_MODE 2
#define SET_BUTTON_MINUS 3
#define SET_BUTTON_PLUS 4
#define TRAY_LEFT 11
#define TRAY_RIGHT 10
#define MOTOR 6
#define HEATER 5
#define COOLING_FAN 7

#define TEMP_HUMIDITY_SUCCESS 101
#define TEMP_HUMIDITY_ERROR 100

#define RTC_SUCCESS 200
#define RTC_ERROR 201

#define	NO_INTERRUPT_MODE 0
#define TEMPERATURE_MODE 1
#define TIMER_MODE 2
#define ROLLING_TIME_MODE 3
#define START_STOP_MODE 4
#define COUNT_MODES 5

#define TIMER_DELTA 1
#define ROLLING_TIME_DELTA 1
#define TEMPERATURE_DELTA 0.5

#define TEMPERATURE_SET_POINT_DEFAULT 66
#define TIMER_DEFAULT 222
#define ROLLING_TIME_DEFAULT 99

extern unsigned int Timer; //In Minutes //Wait time to roll the egg
extern float Temperature_SetPoint; //In Degrees or Farenheit as the value selected.
extern unsigned int RollingTime; //In Seconds
extern bool IsCelcius; //Set Celcius or Farenheit
extern float TemperatureVariation; //In Celcius or Farenheit
extern unsigned int CurrentInterruptMode; //Set the Temperature
extern bool IsStarted;
extern bool IsTransitionedOnOff;

extern void InitializeValues();
#endif

