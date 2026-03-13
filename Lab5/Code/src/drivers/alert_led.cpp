#include "alert_led.h"

#include <Arduino.h>

#include "app_config.h"

void alertLedInit() {
  pinMode(ANALOG_ALERT_LED_PIN, OUTPUT);
  pinMode(DIGITAL_ALERT_LED_PIN, OUTPUT);
  digitalWrite(ANALOG_ALERT_LED_PIN, LOW);
  digitalWrite(DIGITAL_ALERT_LED_PIN, LOW);
}

void alertLedSetAnalog(bool active) {
  digitalWrite(ANALOG_ALERT_LED_PIN, active ? HIGH : LOW);
}

void alertLedSetDigital(bool active) {
  digitalWrite(DIGITAL_ALERT_LED_PIN, active ? HIGH : LOW);
}
