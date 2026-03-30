#ifndef ACTUATOR_LED_H
#define ACTUATOR_LED_H

#include <Arduino.h>

constexpr uint8_t ACTUATOR_LED_PIN = 13U;

void actuatorLedInit();
void actuatorLedSetState(bool enabled);
bool actuatorLedGetState();

#endif
