// 
// 
// 

#include "ButtonsPins.h"
#include "Display.h"
#include "GlobalConstantsVariables.h"
#include "Persistence.h"

unsigned long cMillis;
unsigned long sMillis;
bool IsTimerInterruptAttached = false;
bool BlinkToggle = true;
bool PreviousPlusButtonState = HIGH;
bool PreviousMinusButtonState = HIGH;
bool CurrentButtonState = LOW;

unsigned int TimerTemp = 0;
unsigned int RollingTimeTemp = 0;
float TemperatureSetPointTemp = 0.0;
bool IsStartedTemp = false;

void HandleValueSet(unsigned int whatToSet);
void HandleInterruptDisplayMode();
void UpdateValues();

void ResetMotor();

void PerformLogicForSet()
{
	cMillis = millis();
	//Mode button is intended to be pressed once in 200 milliseconds
	//Otherwise it is blocked.
	if (cMillis - sMillis > 400)
	{
		sMillis = cMillis;
		CurrentInterruptMode = (++CurrentInterruptMode) % COUNT_MODES;
#if DEBUG == 2
		Serial.print("Current Interrupt Mode : ");
		Serial.println(CurrentInterruptMode);
#endif
		if (IsTimerInterruptAttached &&
			CurrentInterruptMode == NO_INTERRUPT_MODE)
		{
			Timer1.detachInterrupt();
			UpdateValues();
			ResetMotor();
			IsTimerInterruptAttached = false;
		}
		else if (!IsTimerInterruptAttached &&
			CurrentInterruptMode != NO_INTERRUPT_MODE)
		{
			Timer1.attachInterrupt(HandleInterruptDisplayMode);
			TimerTemp = Timer;
			RollingTimeTemp = RollingTime;
			TemperatureSetPointTemp = Temperature_SetPoint;
			IsStartedTemp = IsStarted;
			IsTimerInterruptAttached = true;
		}
	}
}

void ResetMotor()
{
	if (!IsStarted)
	{
		digitalWrite(MOTOR, LOW);
	}
	/*if(IsTransitionedOnOff)
	{
#if DEBUG == 1
		Serial.println("Reached to set");
#endif
		IsTransitionedOnOff = false;
		SetTime();
	}*/
}

void UpdateValues()
{
	Temperature_SetPoint = TemperatureSetPointTemp;
	RollingTime = RollingTimeTemp;
	Timer = TimerTemp;
	IsTransitionedOnOff = IsStartedTemp != IsStarted;
	IsStarted = IsStartedTemp;
	WriteToROM(0x000A, Temperature_SetPoint, Timer, RollingTime, IsStarted);

#if DEBUG == 1
	IsTransitionedOnOff ? Serial.println("Transitioned") : Serial.println("Not Transitioned");
#endif
}

void HandleInterruptDisplayMode()
{
#if DEBUG == 2
	unsigned int count = 1;
	Serial.print("Handling Timer Interrupt : ");
	Serial.println(count++);
#endif 
	switch (CurrentInterruptMode)
	{
	case TEMPERATURE_MODE:
	{
		BlinkToggle ? DisplayBlinkVariable("111.", 4) : ClearDisplay();
		BlinkToggle ? DisplayTemperature(TemperatureSetPointTemp) : ClearDisplay();

		break;
	}
	case TIMER_MODE:
	{
		BlinkToggle ? DisplayBlinkVariable("222.", 4) : ClearDisplay();
		BlinkToggle ? DisplayTimer(TimerTemp) : ClearDisplay();
		break;
	}
	case ROLLING_TIME_MODE:
	{
		BlinkToggle ? DisplayBlinkVariable("333.", 4) : ClearDisplay();
		BlinkToggle ? DisplayRollingTime(RollingTimeTemp) : ClearDisplay();
		break;
	}
	case START_STOP_MODE:
	{
		BlinkToggle ? DisplayBlinkVariable("444.", 4) : ClearDisplay();
		BlinkToggle ? DisplayStartStop(IsStartedTemp) : ClearDisplay();
		break;
	}

	default:
		break;
	}
	BlinkToggle = !BlinkToggle;
}

void HandleCurrentInterruptMode()
{
	switch (CurrentInterruptMode)
	{
	case NO_INTERRUPT_MODE:
	{
		break;
	}
	case TEMPERATURE_MODE:
	case TIMER_MODE:
	case ROLLING_TIME_MODE:
	case START_STOP_MODE:
	{
		HandleValueSet(CurrentInterruptMode);
		break;
	}
	default:
		break;
	}
}

void HandleValueSet(unsigned int whatToSet)
{
	CurrentButtonState = digitalRead(SET_BUTTON_MINUS);
	if (CurrentButtonState == LOW
		&& PreviousMinusButtonState == HIGH)
	{
#if DEBUG ==1
		Serial.println("Minus Pressed");
#endif
		switch (whatToSet)
		{
		case TEMPERATURE_MODE:
		{
			TemperatureSetPointTemp -= TEMPERATURE_DELTA;
			break;
		}
		case TIMER_MODE:
		{
			if (TimerTemp >= TIMER_DELTA + 1)
				TimerTemp -= TIMER_DELTA;
			break;
		}
		case ROLLING_TIME_MODE:
		{
			if (RollingTimeTemp >= ROLLING_TIME_DELTA + 1)
				RollingTimeTemp -= ROLLING_TIME_DELTA;
			break;
		}
		case START_STOP_MODE:
		{
			IsStartedTemp = false;
			break;
		}
		default: break;
		}
		PreviousMinusButtonState = LOW;
	}
	if (CurrentButtonState == HIGH)
	{
		PreviousMinusButtonState = HIGH;
	}

	CurrentButtonState = digitalRead(SET_BUTTON_PLUS);
	if (CurrentButtonState == LOW
		&& PreviousPlusButtonState == HIGH)
	{
#if DEBUG ==1
		Serial.println("Plus Pressed");
#endif
		switch (whatToSet)
		{
		case TEMPERATURE_MODE:
		{
			TemperatureSetPointTemp += TEMPERATURE_DELTA;
			break;
		}
		case TIMER_MODE:
		{
			TimerTemp += TIMER_DELTA;
			break;
		}
		case ROLLING_TIME_MODE:
		{
			RollingTimeTemp += ROLLING_TIME_DELTA;
			break;
		}
		case START_STOP_MODE:
		{
			IsStartedTemp = true;
			break;
		}
		default: break;
		}
		PreviousPlusButtonState = LOW;
	}
	if (CurrentButtonState == HIGH)
	{
		PreviousPlusButtonState = HIGH;
	}
}

void InitializeButtonsPins()
{
	//Initialize all input pins
	pinMode(SET_BUTTON_MINUS, INPUT_PULLUP);
	pinMode(SET_BUTTON_PLUS, INPUT_PULLUP);
	pinMode(SET_BUTTON_MODE, INPUT_PULLUP);
	pinMode(TRAY_LEFT, INPUT_PULLUP);
	pinMode(TRAY_RIGHT, INPUT_PULLUP);

	//Initialize all output pins
	pinMode(MOTOR, OUTPUT);
	pinMode(COOLING_FAN, OUTPUT);
	pinMode(HEATER, OUTPUT);
	sMillis = millis();
	attachInterrupt(digitalPinToInterrupt(SET_BUTTON_MODE), PerformLogicForSet, FALLING);
#if DEBUG == 1
	Serial.println("Interrupt Attached");
#endif
	//Cooling Fan is always ON //Relay Connected to NC
	digitalWrite(COOLING_FAN, LOW);

	//Initialize Timer Interrupt but do not attach as of now
	Timer1.initialize(300000);
}

void RegulateFanAndBulb(float* temperature)
{
	// Heating Bulb Logic
	if (*temperature > (Temperature_SetPoint + TemperatureVariation))
	{
#if DEBUG == 1
		Serial.println("Heater is turned OFF");
#endif
		digitalWrite(HEATER, LOW);
	}
	else if (*temperature < (Temperature_SetPoint - TemperatureVariation))
	{
#if DEBUG == 1
		Serial.println("Heater is turned ON");
#endif
		digitalWrite(HEATER, HIGH);
	}

	//Pull to zero Logic
	if (*temperature == Temperature_SetPoint)
	{
#if DEBUG == 1
		Serial.println("Heater pulled to OFF");
#endif
		digitalWrite(HEATER, LOW);
	}
}
