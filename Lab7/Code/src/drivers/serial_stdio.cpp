#include "drivers/serial_stdio.h"

#include <stdio.h>

namespace {

int serialPutChar(char character, FILE *stream) {
  (void)stream;

  if (character == '\n') {
    Serial.write('\r');
  }
  Serial.write(character);
  return 0;
}

int serialGetChar(FILE *stream) {
  (void)stream;
  while (!Serial.available()) {
  }
  return Serial.read();
}

FILE g_serialStream;

}  // namespace

void serialStdioInit(unsigned long baudRate) {
  Serial.begin(baudRate);
  while (!Serial) {
  }

  fdev_setup_stream(&g_serialStream, serialPutChar, serialGetChar, _FDEV_SETUP_RW);
  stdin = &g_serialStream;
  stdout = &g_serialStream;
  stderr = &g_serialStream;
}
