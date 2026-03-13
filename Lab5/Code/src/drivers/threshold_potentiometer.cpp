#include "threshold_potentiometer.h"

#include <Arduino.h>

#include "app_config.h"

void thresholdPotentiometerInit() {
  if (!THRESHOLD_POT_CONNECTED) {
    return;
  }

  pinMode(THRESHOLD_POT_PIN, INPUT);
}

uint16_t thresholdPotentiometerReadRaw() {
  if (!THRESHOLD_POT_CONNECTED) {
    return ADC_MAX_VALUE / 2U;
  }

  return static_cast<uint16_t>(analogRead(THRESHOLD_POT_PIN));
}
