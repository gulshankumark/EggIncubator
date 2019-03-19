// 
// 
// 

#include "TempHumidity.h"
#include "GlobalConstantsVariables.h"

SimpleDHT22 dht22(TEMP_HUMIDITY_PIN);

int ReadTemperatureHumidity(float* temperature, float* humidity, int* err)
{
	*err = SimpleDHTErrSuccess;
	if ((*err = dht22.read2(temperature, humidity, NULL)) != SimpleDHTErrSuccess)
	{
		return TEMP_HUMIDITY_ERROR;
	}
	if (!IsCelcius)
	{
		*temperature = ((*temperature * 9) / (float)5) + 32;
	}
	return TEMP_HUMIDITY_SUCCESS;
}
