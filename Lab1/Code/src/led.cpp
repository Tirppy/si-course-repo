#include "led.h"

static uint8_t ledPin = 0;
static uint8_t ledState = LOW;

void ledInit(uint8_t pin) {
    ledPin = pin;
    ledState = LOW;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void ledOn(void) {
    ledState = HIGH;
    digitalWrite(ledPin, HIGH);
}

void ledOff(void) {
    ledState = LOW;
    digitalWrite(ledPin, LOW);
}

uint8_t ledGetState(void) {
    return ledState;
}
