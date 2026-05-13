#include "drivers/relay_actuator.h"

#include <Arduino.h>

#include "config/pins.h"

void heaterOutputInit() {
  pinMode(kHeaterOutputPin, OUTPUT);
  digitalWrite(kHeaterOutputPin, LOW);
}

void heaterOutputApply(ActuatorState state) {
  digitalWrite(kHeaterOutputPin, state == ActuatorState::On ? HIGH : LOW);
}
