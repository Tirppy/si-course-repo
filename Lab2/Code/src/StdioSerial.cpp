#include <Arduino.h>
#include <stdio.h>

#include "StdioSerial.h"

namespace {
FILE serialOutputStream;
FILE serialInputStream;

int serialPutChar(char character, FILE*) {
    if (character == '\n') {
        Serial.write('\r');
    }

    Serial.write(character);
    return 0;
}

int serialGetChar(FILE*) {
    while (!Serial.available()) {
    }

    return Serial.read();
}
} // namespace

void initializeStdioSerial() {
    fdev_setup_stream(&serialOutputStream, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
    stdout = &serialOutputStream;

    fdev_setup_stream(&serialInputStream, nullptr, serialGetChar, _FDEV_SETUP_READ);
    stdin = &serialInputStream;
}
