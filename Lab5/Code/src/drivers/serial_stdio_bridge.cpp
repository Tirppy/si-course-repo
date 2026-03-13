#include "serial_stdio_bridge.h"

#include <Arduino.h>
#include <stdio.h>

namespace {
int serialPutchar(char c, FILE *) {
  if (c == '\n') {
    Serial.write('\r');
  }
  Serial.write(c);
  return 0;
}

FILE g_serialStdout = {};
}  // namespace

void serialStdioBridgeInit() {
  Serial.begin(9600);
  while (!Serial) {
  }

  fdev_setup_stream(&g_serialStdout, serialPutchar, nullptr, _FDEV_SETUP_WRITE);
  stdout = &g_serialStdout;
}
