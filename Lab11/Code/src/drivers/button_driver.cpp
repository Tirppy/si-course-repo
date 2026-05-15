#include "button_driver.h"

#include <Arduino.h>

namespace {
constexpr uint8_t kButtonPin = 12U;
}

void buttonDriverSetup() {
  pinMode(kButtonPin, INPUT_PULLUP);
}

bool buttonDriverIsPressed() {
  return digitalRead(kButtonPin) == LOW;
}
