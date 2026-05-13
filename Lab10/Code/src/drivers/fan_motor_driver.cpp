#include "drivers/fan_motor_driver.h"

#include <Arduino.h>

#include "config/pins.h"

namespace {

uint8_t g_fanDutyPercent = 0U;

void stopMotor() {
  digitalWrite(kMotorDriverIn1Pin, LOW);
  digitalWrite(kMotorDriverIn2Pin, LOW);
}

}  // namespace

void fanMotorInit() {
  pinMode(kMotorDriverIn1Pin, OUTPUT);
  pinMode(kMotorDriverIn2Pin, OUTPUT);
  stopMotor();
}

void fanMotorSetDutyPercent(uint8_t dutyPercent) {
  if (dutyPercent > 100U) {
    dutyPercent = 100U;
  }

  g_fanDutyPercent = dutyPercent;

  if (g_fanDutyPercent == 0U) {
    stopMotor();
    return;
  }

  digitalWrite(kMotorDriverIn1Pin, LOW);
  analogWrite(kMotorDriverIn2Pin, map(g_fanDutyPercent, 0, 100, 0, 255));
}

void fanMotorService() {}
