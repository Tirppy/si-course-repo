#include "signal_conversion.h"

#include <Arduino.h>
#include <math.h>

#include "app_config.h"

float ntcAdcToCelsius(int rawAdc) {
  if (rawAdc <= 0) {
    rawAdc = 1;
  }
  if (rawAdc >= static_cast<int>(ADC_MAX_VALUE)) {
    rawAdc = static_cast<int>(ADC_MAX_VALUE) - 1;
  }

  const float ratio = (static_cast<float>(ADC_MAX_VALUE) / static_cast<float>(rawAdc)) - 1.0F;
  const float inverseTemperatureK = (logf(1.0F / ratio) / NTC_BETA) + (1.0F / NTC_ROOM_TEMP_K);

  return (1.0F / inverseTemperatureK) - 273.15F;
}

float mapPotentiometerToThresholdC(int rawAdc) {
  const long constrainedValue = constrain(rawAdc, 0, static_cast<int>(ADC_MAX_VALUE));
  return ALERT_THRESHOLD_MIN_C +
         (static_cast<float>(constrainedValue) * (ALERT_THRESHOLD_MAX_C - ALERT_THRESHOLD_MIN_C) /
          static_cast<float>(ADC_MAX_VALUE));
}

bool isAnalogTemperatureValid(float temperatureC) {
  return !isnan(temperatureC) && temperatureC >= ANALOG_VALID_MIN_C &&
         temperatureC <= ANALOG_VALID_MAX_C;
}

bool isDigitalTemperatureValid(float temperatureC) {
  return !isnan(temperatureC) && temperatureC >= DIGITAL_VALID_MIN_C &&
         temperatureC <= DIGITAL_VALID_MAX_C;
}
