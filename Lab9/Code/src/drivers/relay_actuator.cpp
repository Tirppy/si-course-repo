#include "drivers/relay_actuator.h"

#include <Arduino.h>

#include "config/pins.h"

void actuatorOutputInit() {
  pinMode(kActuatorLedPin, OUTPUT);
  digitalWrite(kActuatorLedPin, LOW);
}

void actuatorOutputApply(ActuatorState state) {
  digitalWrite(kActuatorLedPin, state == ActuatorState::On ? HIGH : LOW);
}
