#ifndef HARDWARE_INPUTS_H
#define HARDWARE_INPUTS_H

#include <Arduino.h>

constexpr uint8_t BUTTON_INPUT_PIN = 2U;
constexpr uint8_t POTENTIOMETER_PIN = A0;

void hardwareInputsInit();
bool hardwareInputsReadButtonPressed();
uint8_t hardwareInputsReadPotPercent();

#endif
