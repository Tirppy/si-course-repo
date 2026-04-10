#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

constexpr uint8_t MOTOR_ENABLE_PIN = 10U;
constexpr uint8_t MOTOR_IN1_PIN = 8U;
constexpr uint8_t MOTOR_IN2_PIN = 9U;

void motorDriverInit();
void motorDriverSetSpeedPercent(uint8_t percent);
uint8_t motorDriverGetSpeedPercent();
void motorDriverSetReverse(bool reversed);
bool motorDriverIsReverse();
bool motorDriverTick();

#endif
