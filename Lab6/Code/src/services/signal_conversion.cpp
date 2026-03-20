#include "signal_conversion.h"

#include <math.h>

#include "app_config.h"

float ntcAdcToCelsius(float rawAdc) {
  if (rawAdc <= 0.0F) {
    rawAdc = 1.0F;
  }
  if (rawAdc >= static_cast<float>(ADC_MAX_VALUE)) {
    rawAdc = static_cast<float>(ADC_MAX_VALUE - 1U);
  }

  const float resistanceRatio = (static_cast<float>(ADC_MAX_VALUE) / rawAdc) - 1.0F;
  const float inverseTemperatureK = (logf(resistanceRatio) / NTC_BETA) + (1.0F / NTC_ROOM_TEMP_K);

  return (1.0F / inverseTemperatureK) - 273.15F;
}

bool isAnalogTemperatureValid(float temperatureC) {
  return !isnan(temperatureC) && temperatureC >= ANALOG_VALID_MIN_C &&
         temperatureC <= ANALOG_VALID_MAX_C;
}

bool isDigitalTemperatureValid(float temperatureC) {
  return !isnan(temperatureC) && temperatureC >= DIGITAL_VALID_MIN_C &&
         temperatureC <= DIGITAL_VALID_MAX_C;
}
