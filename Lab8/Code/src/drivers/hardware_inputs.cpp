#include "drivers/hardware_inputs.h"

void hardwareInputsInit() {
  pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);
}

bool hardwareInputsReadButtonPressed() {
  return digitalRead(BUTTON_INPUT_PIN) == LOW;
}

uint8_t hardwareInputsReadPotPercent() {
  const int adcValue = analogRead(POTENTIOMETER_PIN);
  int normalizedValue = adcValue;
  if (normalizedValue <= 20) {
    normalizedValue = 0;
  } else if (normalizedValue >= 1003) {
    normalizedValue = 1023;
  } else {
    normalizedValue = map(normalizedValue, 21, 1002, 0, 1023);
  }

  const long percent = map(normalizedValue, 0, 1023, 0, 100);
  return static_cast<uint8_t>(percent);
}
