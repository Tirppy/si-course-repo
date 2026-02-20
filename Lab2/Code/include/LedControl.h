#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

class LedControl {
public:
    LedControl(uint8_t greenPin, uint8_t redPin);

    void initialize();
    void showValid();
    void showInvalid();
    void turnOff();

private:
    uint8_t greenLedPin;
    uint8_t redLedPin;
};

#endif
