#include "drivers/indicator_led.h"

namespace {

bool g_indicatorLedState = false;

}  // namespace

void indicatorLedInit() {
  pinMode(INDICATOR_LED_PIN, OUTPUT);
  indicatorLedSetState(false);
}

void indicatorLedSetState(bool enabled) {
  g_indicatorLedState = enabled;
  digitalWrite(INDICATOR_LED_PIN, enabled ? HIGH : LOW);
}

bool indicatorLedGetState() {
  return g_indicatorLedState;
}
