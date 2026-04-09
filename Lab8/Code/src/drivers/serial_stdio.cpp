#include "drivers/serial_stdio.h"

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

FILE g_serialStream;

}  // namespace

void serialStdioInit(unsigned long baudRate) {
  Serial.begin(baudRate);
  while (!Serial) {
  }

  fdev_setup_stream(&g_serialStream, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
  stdout = &g_serialStream;
}
