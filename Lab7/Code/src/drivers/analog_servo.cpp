#include "drivers/analog_servo.h"

namespace {

uint8_t g_pwmValue = 0U;

}  // namespace

void analogServoInit() {
  pinMode(ANALOG_SERVO_PIN, OUTPUT);
  analogServoWriteDegrees(0U);
}

void analogServoWriteDegrees(uint8_t pwmValue) {
  g_pwmValue = pwmValue;
  analogWrite(ANALOG_SERVO_PIN, pwmValue);
}

uint8_t analogServoGetDegrees() {
  return g_pwmValue;
}
