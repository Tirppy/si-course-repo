#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

constexpr uint8_t MOTOR_PWM_PIN = 10U;
constexpr uint8_t MOTOR_DIRECTION_PIN = 11U;

void motorDriverInit();
void motorDriverSetSpeedPercent(uint8_t percent);
uint8_t motorDriverGetSpeedPercent();
void motorDriverSetReverse(bool reversed);
bool motorDriverIsReverse();
bool motorDriverTick();

#endif
