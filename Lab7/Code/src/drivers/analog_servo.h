#ifndef ANALOG_SERVO_H
#define ANALOG_SERVO_H

#include <Arduino.h>

constexpr uint8_t ANALOG_SERVO_PIN = 10U;

void analogServoInit();
void analogServoWriteDegrees(uint8_t pwmValue);
uint8_t analogServoGetDegrees();

#endif
