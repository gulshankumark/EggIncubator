// Persistence.h

#ifndef _PERSISTENCE_h
#define _PERSISTENCE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <EEPROM.h>

//Structure for EEPROM Memory Write
//Each Line Represents a Byte
//#########Starts Here############
// 0x0000 Constant value 0xF2 (242)
// 0x0001 Constant value 0xEA (234)
// 0x0002 Length in bytes (MSB)
// 0x0003 Length in bytes (LSB)
// 0x0004 Temperature (MSB)
// 0x0005 ...
// 0x0006 ...
// 0x0007 Temperature (LSB)
// 0x0008 Timer (MSB)
// 0x0009 Timer (LSB)
// 0X000A Rolling Time (MSB)
// 0X000B Rolling Time (LSB)
// 0x000C Start (0x99 for Start) (0xA8 for Stop)
// 0x000D End Value (0x05)

extern void InitializeDefaultValues();
extern void WriteToROM(unsigned int length, float temperature, unsigned int timer,
	unsigned int rollingTime, bool toStart);
#endif

