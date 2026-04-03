#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

constexpr uint8_t STEPPER_IN1_PIN = 8U;
constexpr uint8_t STEPPER_IN2_PIN = 9U;
constexpr uint8_t STEPPER_IN3_PIN = 10U;
constexpr uint8_t STEPPER_IN4_PIN = 11U;
constexpr int STEPPER_STEPS_PER_REVOLUTION = 2048;
constexpr unsigned long STEPPER_SLOWEST_INTERVAL_US = 120000UL;
constexpr unsigned long STEPPER_FASTEST_INTERVAL_US = 2500UL;

void motorDriverInit();
void motorDriverSetSpeedPercent(uint8_t percent);
uint8_t motorDriverGetSpeedPercent();
void motorDriverSetReverse(bool reversed);
bool motorDriverIsReverse();
bool motorDriverTick();

#endif
