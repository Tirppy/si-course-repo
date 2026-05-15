#include "serial_stdio.h"

#include <Arduino.h>
#include <stdio.h>

namespace {

int serialPutChar(char ch, FILE *stream) {
  (void)stream;
  if (ch == '\n') {
    Serial.write('\r');
  }
  Serial.write(ch);
  return 0;
}

int serialGetChar(FILE *stream) {
  (void)stream;
  while (!Serial.available()) {
  }
  return Serial.read();
}

}  // namespace

void serialStdioSetup(uint32_t baudRate) {
  Serial.begin(baudRate);
  FILE *serialStream = fdevopen(serialPutChar, serialGetChar);
  stdin = serialStream;
  stdout = serialStream;
  stderr = serialStream;
}
