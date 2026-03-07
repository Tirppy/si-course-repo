#include "stdio_bridge.h"

#include <Arduino.h>
#include <stdio.h>

namespace {
int serialPutCharacter(char character, FILE*) {
  if (character == '\n') {
    Serial.write('\r');
  }
  Serial.write(static_cast<uint8_t>(character));
  return 0;
}

FILE g_stdoutStream;
}

void stdioBridgeInit(uint32_t baudRate) {
  Serial.begin(baudRate);

  fdev_setup_stream(&g_stdoutStream, serialPutCharacter, nullptr, _FDEV_SETUP_WRITE);
  stdout = &g_stdoutStream;
}