// 
// 
// 

#include "RTCLogic.h"
#include "GlobalConstantsVariables.h"


RTC_DS1307 rtc_ds1307;
DateTime StartDateTime = DateTime(2019, 3, 14, 0, 0, 0);
unsigned int elapsedSeconds;
unsigned int elapsedSecondsRTC;
unsigned int rollingElapsedSeconds;
unsigned long rollingMillis;
bool isMotorOn = false;
char DaysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void InitializeRTC()
{
	if (!rtc_ds1307.begin()) {
#if DEBUG == 1 
		Serial.println("Couldn't find RTC");
#endif 
		while (1);
	}

	if (!rtc_ds1307.isrunning()) {
#if DEBUG == 1
		Serial.println("RTC is NOT running!");
#endif

		// following line sets the RTC to the date & time this sketch was compiled
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
}

unsigned int CalculateElapsedSeconds()
{
	return  (rtc_ds1307.now() - StartDateTime).totalseconds();
}

void DoMotorOperation(unsigned long* startMillis, unsigned long* currentMillis)
{
	elapsedSeconds = (*currentMillis - *startMillis) / 1000;
	if (elapsedSeconds % 5 == 0 && !isMotorOn)
	{
		elapsedSecondsRTC = CalculateElapsedSeconds();
#if DEBUG == 1
		Serial.println("Each 5 seconds");
		Serial.print("Elapsed RTC: ");
		Serial.println(elapsedSecondsRTC);
#endif
		if (elapsedSecondsRTC > Timer * 60)
		{
			*startMillis = *currentMillis;
			rollingMillis = *currentMillis;
			digitalWrite(MOTOR, HIGH);
			isMotorOn = true;
		}
	}
	if (isMotorOn)
	{
		rollingElapsedSeconds = (*currentMillis - rollingMillis) / 1000;
#if DEBUG == 1
		Serial.println("Rolling Time: ");
		Serial.println(rollingElapsedSeconds);
#endif
		if (rollingElapsedSeconds > RollingTime)
		{
			digitalWrite(MOTOR, LOW);
			SetTime();
			isMotorOn = false;
		}
	}
}

DateTime GetCurrentDateTime()
{
	return rtc_ds1307.now();
}

void SetTime()
{
#if DEBUG == 1
	Serial.println("Time is to be Set. Wait 2 Seconds...");
#endif
	delay(2000);
	rtc_ds1307.adjust(StartDateTime);
}
