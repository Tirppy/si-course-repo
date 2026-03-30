#include "drivers/actuator_led.h"

namespace {
bool g_ledState = false;
}

void actuatorLedInit() {
  pinMode(ACTUATOR_LED_PIN, OUTPUT);
  actuatorLedSetState(false);
}

void actuatorLedSetState(bool enabled) {
  g_ledState = enabled;
  digitalWrite(ACTUATOR_LED_PIN, enabled ? HIGH : LOW);
}

bool actuatorLedGetState() {
  return g_ledState;
}
