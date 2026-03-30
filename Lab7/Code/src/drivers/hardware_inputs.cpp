#include "drivers/hardware_inputs.h"

void hardwareInputsInit() {
  pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);
}

bool hardwareInputsReadButtonPressed() {
  return digitalRead(BUTTON_INPUT_PIN) == LOW;
}

uint8_t hardwareInputsReadPotPercent() {
  const int adcValue = analogRead(POTENTIOMETER_PIN);
  const long percent = map(adcValue, 0, 1023, 0, 100);
  return static_cast<uint8_t>(percent);
}
