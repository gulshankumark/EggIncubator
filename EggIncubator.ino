#include "Persistence.h"
#include "Display.h"
#include "ButtonsPins.h"
#include "RTCLogic.h"
#include "TempHumidity.h"
#include "GlobalConstantsVariables.h"


//Global Variables
int status;
float temperature;
float humidity;
int err_dht22;
unsigned long startMillis;
unsigned long currentMillis;
unsigned int toggleBlink = 0;

//Declarations of the local functions
void ReadTemperatureHumidityInLoop();
void HandleInterruptMode();
void HandleNonInterruptMode();

void setup()
{
#if DEBUG == 1
	Serial.begin(115200);
#endif
	InitializeValues();
	InitialDisplay();
	InitializeRTC();
	InitializeButtonsPins();
	startMillis = millis();
}


// Add the main program code into the continuous loop() function
void loop()
{
	if (CurrentInterruptMode == NO_INTERRUPT_MODE)
	{
		HandleNonInterruptMode();
	}
	else
	{
		HandleInterruptMode();
	}
}


void HandleNonInterruptMode()
{

#if DEBUG == 1	
	DateTime datetime = GetCurrentDateTime();
	Serial.print(datetime.day());
	Serial.print("/");
	Serial.print(datetime.month());
	Serial.print("/");
	Serial.print(datetime.year());
	Serial.print("/");
	Serial.print(" -- ");
	Serial.print(datetime.hour());
	Serial.print(":");
	Serial.print(datetime.minute());
	Serial.print(":");
	Serial.println(datetime.second());
#endif
	if (IsTransitionedOnOff)
	{
#if DEBUG == 1
		Serial.println("Reached to set");
#endif
		IsTransitionedOnOff = false;
		SetTime();
	}

	ReadTemperatureHumidityInLoop();
	RegulateFanAndBulb(&temperature);
	if (IsStarted)
	{
		DisplayTemperatureHumidity(temperature, humidity);
		DoMotorOperation(&startMillis, &currentMillis);
	}
	else
	{
		switch (toggleBlink)
		{
		case 0:
		{
			DisplayUpdateVal();
			break;
		}
		case 2:
		{
			DisplayTemperatureHumidity(temperature, humidity);
			break;
		}
		default:
		{
			ClearDisplay();
			break;
		}
		}
		toggleBlink = (++toggleBlink) % 4;
	}

	currentMillis = millis();
	delay(1000);
}

void HandleInterruptMode()
{
	HandleCurrentInterruptMode();
}

void ReadTemperatureHumidityInLoop()
{

#if DEBUG == 1
	Serial.println("=================================");
	Serial.println("Sample DHT22...");
#endif
	status = ReadTemperatureHumidity(&temperature, &humidity, &err_dht22);
	if (status == TEMP_HUMIDITY_ERROR)
	{
#if DEBUG == 1
		Serial.print("Read DHT22 failed, err="); Serial.println(err_dht22);
#endif
	}
	else
	{
#if DEBUG == 1
		Serial.print("Sample OK: ");
		Serial.print(static_cast<float>(temperature));
		IsCelcius ? Serial.print(" *C, ") : Serial.print(" *F, ");
		Serial.print(static_cast<float>(humidity)); Serial.println(" RH%");
#endif // DEBUG ==1

	}
}
