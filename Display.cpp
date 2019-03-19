// 
// 
// 

#include "Display.h"
#include "GlobalConstantsVariables.h"

LedControl lc = LedControl(DISPLAY_DIN, DISPLAY_CLK, DISPLAY_CS, 1);

void InitialDisplay()
{
	lc.shutdown(0, false);
	/* Set the brightness to a medium values */
	lc.setIntensity(0, 8);
	/* and clear the display */
	lc.clearDisplay(0);
}

void ClearDisplay()
{
	lc.clearDisplay(0);
}

void DisplayBlinkVariable(char* str, int length)
{
	lc.clearDisplay(0);
	int digit = 7;
	for (int i = 0; i < length || i < 4; i++)
	{
		lc.setChar(0, digit--, str[i], false);
	}
}

void DisplayTemperatureHumidity(float temperature, float humidity)
{
	DisplayTemperature(temperature);
	DisplayHumidity(humidity);
}

void DisplayTemperature(float temperature)
{
	int newValue = (int)(temperature * 10) % 1000;
#if DEBUG == 1
	Serial.print("Temperature Converted: ");
	Serial.println(newValue);
#endif
	int digit = 0;
	if (IsCelcius)
	{
		lc.setChar(0, digit++, 'C', false);
	}
	else
	{
		lc.setChar(0, digit++, 'F', false);
	}
	while (newValue > 0)
	{
		lc.setDigit(0, digit++, newValue % 10, digit == 2);
		newValue = newValue / 10;
	}
}

void DisplayHumidity(float humidity)
{
	int newValue = (int)(humidity * 10) % 1000;
#if DEBUG == 1
	Serial.print("Humidity Converted: ");
	Serial.println(newValue);
#endif
	int digit = 4;
	lc.setChar(0, digit++, 'H', false);
	while (newValue > 0)
	{
		lc.setDigit(0, digit++, newValue % 10, digit == 6);
		newValue = newValue / 10;
	}
}

void DisplayTimer(unsigned int timer)
{
#if DEBUG == 1
	Serial.print("Timer: ");
	Serial.println(timer);
#endif
	int digit = 0;
	lc.setChar(0, digit++, '_', false);
	while (timer > 0)
	{
		lc.setDigit(0, digit++, timer % 10, false);
		timer = timer / 10;
	}
}

void DisplayRollingTime(unsigned int rollingTime)
{
#if DEBUG == 1
	Serial.print("Rolling Time: ");
	Serial.println(rollingTime);
#endif
	int digit = 0;
	lc.setChar(0, digit++, '-', false);
	while (rollingTime > 0)
	{
		lc.setDigit(0, digit++, rollingTime % 10, false);
		rollingTime = rollingTime / 10;
	}
}

void DisplayStartStop(bool startStop)
{
#if DEBUG == 1
	Serial.print("Started: ");
	if (startStop)
		Serial.println("Yes");
	else
		Serial.println("No");
#endif
	int digit = 3;
	if(startStop)
	{
		lc.setChar(0, digit--, '0', false);
		lc.setChar(0, digit--, 'H', false);
		lc.setChar(0, digit--, '.', false);
		lc.setChar(0, digit, '.', false);
	}
	else
	{
		lc.setChar(0, digit--, '0', false);
		lc.setChar(0, digit--, 'F', false);
		lc.setChar(0, digit--, 'F', false);
		lc.setChar(0, digit, '.', false);
	}
}

void DisplayUpdateVal()
{
	int digit = 7;
	lc.setChar(0, digit--, 'c', false);
	lc.setChar(0, digit--, 'P', false);
	lc.setChar(0, digit--, 'd', false);
	lc.setChar(0, digit--, '.', false);
	lc.setChar(0, digit--, 'c', false);
	lc.setChar(0, digit--, 'A', false);
	lc.setChar(0, digit--, 'L', false);
	lc.setChar(0, digit, '.', false);
}
