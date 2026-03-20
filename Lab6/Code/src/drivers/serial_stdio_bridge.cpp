#include "serial_stdio_bridge.h"

#include <Arduino.h>
#include <stdio.h>

namespace {
int serialPutChar(char character, FILE *) {
  if (character == '\n') {
    Serial.write('\r');
  }
  Serial.write(static_cast<uint8_t>(character));
  return 0;
}

FILE g_serialOutput;
}  // namespace

void serialStdioBridgeInit() {
  Serial.begin(9600);
  while (!Serial) {
  }

  fdev_setup_stream(&g_serialOutput, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
  stdout = &g_serialOutput;
}
