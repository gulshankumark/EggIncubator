// 
// 
// 

#include "GlobalConstantsVariables.h"
#include "Persistence.h"

unsigned int Timer; //In Minutes //Wait time to roll the egg
float Temperature_SetPoint; //In Degrees or Farenheit as the value selected.
unsigned int RollingTime; //In Seconds
bool IsCelcius; //Set Celcius or Farenheit
float TemperatureVariation; //In Celcius or Farenheit
unsigned int CurrentInterruptMode;
bool IsStarted; 
bool IsTransitionedOnOff;
void InitializeValues()
{
	InitializeDefaultValues();

	CurrentInterruptMode = NO_INTERRUPT_MODE;
	//Set Initial Values
	/*Timer = 300;
	Temperature_SetPoint = 37.5;
	RollingTime = 5;*/
	IsCelcius = true; 
	IsTransitionedOnOff = false;
	TemperatureVariation = 0.5;
}
