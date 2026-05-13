#ifndef DRIVERS_FAN_MOTOR_DRIVER_H
#define DRIVERS_FAN_MOTOR_DRIVER_H

#include <Arduino.h>

void fanMotorInit();
void fanMotorSetDutyPercent(uint8_t dutyPercent);
void fanMotorService();

#endif
