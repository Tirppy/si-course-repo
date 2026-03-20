#include "alert_led.h"

#include <Arduino.h>

#include "app_config.h"

namespace {
void setLed(uint8_t pin, bool enabled) {
  digitalWrite(pin, enabled ? HIGH : LOW);
}
}  // namespace

void alertLedInit() {
  pinMode(TREND_LED_PIN, OUTPUT);
  pinMode(ANALOG_ALERT_LED_PIN, OUTPUT);
  pinMode(DIGITAL_ALERT_LED_PIN, OUTPUT);
  setLed(TREND_LED_PIN, false);
  setLed(ANALOG_ALERT_LED_PIN, false);
  setLed(DIGITAL_ALERT_LED_PIN, false);
}

void alertLedSetTrend(bool enabled) {
  setLed(TREND_LED_PIN, enabled);
}

void alertLedSetAnalog(bool enabled) {
  setLed(ANALOG_ALERT_LED_PIN, enabled);
}

void alertLedSetDigital(bool enabled) {
  setLed(DIGITAL_ALERT_LED_PIN, enabled);
}
