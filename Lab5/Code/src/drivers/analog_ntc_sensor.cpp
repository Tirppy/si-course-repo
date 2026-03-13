#include "analog_ntc_sensor.h"

#include <Arduino.h>

#include "app_config.h"
#include "signal_conversion.h"

void analogNtcSensorInit() {
  pinMode(ANALOG_NTC_PIN, INPUT);
}

AnalogTemperatureSample analogNtcSensorRead() {
  const int rawAdc = analogRead(ANALOG_NTC_PIN);
  const float temperatureC = ntcAdcToCelsius(rawAdc);

  AnalogTemperatureSample sample = {
      static_cast<uint16_t>(rawAdc),
      temperatureC,
      isAnalogTemperatureValid(temperatureC),
  };

  return sample;
}
