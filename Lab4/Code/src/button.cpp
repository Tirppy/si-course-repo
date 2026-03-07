#include "button.h"

#include <Arduino.h>

void buttonInit() {
  pinMode(2, INPUT_PULLUP);
}

bool buttonIsPressed() {
  return digitalRead(2) == LOW;
}