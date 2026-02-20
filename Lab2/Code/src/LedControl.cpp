#include "LedControl.h"

LedControl::LedControl(uint8_t greenPin, uint8_t redPin)
    : greenLedPin(greenPin), redLedPin(redPin) {}

void LedControl::initialize() {
    pinMode(greenLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    turnOff();
}

void LedControl::showValid() {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
}

void LedControl::showInvalid() {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
}

void LedControl::turnOff() {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, LOW);
}
