#include "drivers/thermistor_sensor.h"

#include <Arduino.h>

#include "config/pins.h"

void thermistorSensorInit() { pinMode(kThermistorPin, INPUT); }

int thermistorSensorReadRaw() { return analogRead(kThermistorPin); }
