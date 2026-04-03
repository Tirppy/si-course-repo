#ifndef INDICATOR_LED_H
#define INDICATOR_LED_H

#include <Arduino.h>

constexpr uint8_t INDICATOR_LED_PIN = 12U;

void indicatorLedInit();
void indicatorLedSetState(bool enabled);
bool indicatorLedGetState();

#endif
