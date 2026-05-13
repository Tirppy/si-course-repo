#include "services/thermistor_math.h"

#include <math.h>

#include "config/constants.h"

float thermistorVoltageFromAdc(int adcValue) {
  return (static_cast<float>(adcValue) / static_cast<float>(kAdcMaxValue)) * kSupplyVoltage;
}

float thermistorResistanceFromAdc(int adcValue) {
  if (adcValue <= 0) {
    return 1000000.0F;
  }

  if (adcValue >= kAdcMaxValue) {
    return 1.0F;
  }

  return kSeriesResistorOhms *
         ((static_cast<float>(kAdcMaxValue) / static_cast<float>(adcValue)) - 1.0F);
}

float thermistorTemperatureCFromAdc(int adcValue) {
  const float resistance = thermistorResistanceFromAdc(adcValue);
  const float nominalTemperatureK = kNominalTemperatureC + kKelvinOffset;
  const float steinhart =
      (1.0F / nominalTemperatureK) +
      (log(resistance / kNominalResistanceOhms) / kBetaCoefficient);

  return (1.0F / steinhart) - kKelvinOffset;
}

bool thermistorReadingValid(int adcValue, float temperatureC) {
  return adcValue > 0 && adcValue < kAdcMaxValue && !isnan(temperatureC) &&
         temperatureC > -20.0F && temperatureC < 120.0F;
}
