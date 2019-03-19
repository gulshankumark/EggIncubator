// 
// 
// 

#include "Persistence.h"
#include "GlobalConstantsVariables.h"

byte firstBytePer;
byte secondBytePer;
unsigned int lengthPer;
float temperaturePer;
unsigned int timerPer;
unsigned int rtimePer;
byte startStopPer;
byte endBytePer;

bool ReadFromROM();

void InitializeDefaultValues()
{
	if(ReadFromROM())
	{
		Temperature_SetPoint = temperaturePer;
		Timer = timerPer;
		RollingTime = rtimePer;
		if (startStopPer == 0x99)
		{
			IsStarted = true;
		}
		else if (startStopPer == 0xA8)
		{
			IsStarted = false;
		}
	}
	else
	{
		Temperature_SetPoint = TEMPERATURE_SET_POINT_DEFAULT;
		Timer = TIMER_DEFAULT;
		RollingTime = ROLLING_TIME_DEFAULT;
		IsStarted = false;
	}
}


bool ReadFromROM()
{
	unsigned int offset = 0;

	firstBytePer = EEPROM.read(offset++);
	if (firstBytePer != 0xF2)
	{
		return  false;
	}
	secondBytePer = EEPROM.read(offset++);
	if (secondBytePer != 0xEA)
	{
		return  false;
	}
	lengthPer = 0;
	temperaturePer = 0.0;
	timerPer = 0;
	rtimePer = 0;
	EEPROM.get(offset, lengthPer);
	offset += 2;
	EEPROM.get(offset, temperaturePer);
	offset += 4;
	EEPROM.get(offset, timerPer);
	offset += 2;
	EEPROM.get(offset, rtimePer);
	offset += 2;
	startStopPer = EEPROM.read(offset++);
	if (!(startStopPer == 0x99 || startStopPer == 0xA8))
	{
		return false;
	}
	endBytePer = EEPROM.read(offset);

#if DEBUG == 1
	Serial.print("Length : ");
	Serial.println(lengthPer);

	Serial.print("Temperature : ");
	Serial.println(temperaturePer);

	Serial.print("Timer : ");
	Serial.println(timerPer);

	Serial.print("Rolling Time : ");
	Serial.println(rtimePer);

	Serial.print("Started : ");
	startStopPer == 0x99 ? Serial.println("YES") : startStopPer == 0xA8 ? Serial.println("NO") : Serial.println("UNDEFINED");

	Serial.print("End Byte : ");
	Serial.println(endBytePer);
#endif
	return endBytePer == 0x05;
}

void WriteToROM(unsigned int length, float temperature, unsigned int timer,
	unsigned int rollingTime, bool toStart)
{
	unsigned int temp = 0;
	unsigned int offset = 0;
	float tempFloat = 0.0;

	//Start Values
	EEPROM.update(offset++, 0xF2);
	EEPROM.update(offset++, 0xEA);

	//Length
	if (EEPROM.get(offset, temp) != length)
	{
		EEPROM.put(offset, length);
	}
	offset += 2;

	//Temperature
	if (EEPROM.get(offset, tempFloat) != temperature)
	{
		EEPROM.put(offset, temperature);
	}
	offset += 4;

	//Timer
	if (EEPROM.get(offset, temp) != timer)
	{
		EEPROM.put(offset, timer);
	}
	offset += 2;

	//Rolling Time
	if (EEPROM.get(offset, temp) != rollingTime)
	{
		EEPROM.put(offset, rollingTime);
	}
	offset += 2;

	//Start/Stop
	if (EEPROM.read(offset) == 0x99 && !toStart)
	{
		EEPROM.write(offset, 0xA8);
	}
	else if (EEPROM.read(offset) == 0xA8 && toStart)
	{
		EEPROM.write(offset, 0x99);
	}
	offset++;

	//End Value
	EEPROM.update(offset, 0x05);
}