#include "analog_thermistor.h"

#include <Arduino.h>

#include "app_config.h"

void analogThermistorInit() {
  pinMode(ANALOG_SENSOR_PIN, INPUT);
}

AnalogTemperatureSample analogThermistorRead() {
  AnalogTemperatureSample sample = {static_cast<uint16_t>(analogRead(ANALOG_SENSOR_PIN))};
  return sample;
}
