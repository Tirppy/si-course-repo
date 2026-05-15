#include "led_driver.h"

#include <Arduino.h>

namespace {
constexpr uint8_t kLedPin = 9U;
bool g_ledEnabled = false;
}

void ledDriverSetup() {
  pinMode(kLedPin, OUTPUT);
  ledDriverSet(false);
}

void ledDriverSet(bool enabled) {
  g_ledEnabled = enabled;
  digitalWrite(kLedPin, enabled ? HIGH : LOW);
}

bool ledDriverIsOn() {
  return g_ledEnabled;
}
