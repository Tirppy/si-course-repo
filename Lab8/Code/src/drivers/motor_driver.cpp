#include "drivers/motor_driver.h"

namespace {

uint8_t g_speedPercent = 0U;
bool g_reversed = false;

}  // namespace

void motorDriverInit() {
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_DIRECTION_PIN, OUTPUT);
  analogWrite(MOTOR_PWM_PIN, 0);
  digitalWrite(MOTOR_DIRECTION_PIN, LOW);
  motorDriverSetSpeedPercent(0U);
}

void motorDriverSetSpeedPercent(uint8_t percent) {
  if (percent > 100U) {
    percent = 100U;
  }

  g_speedPercent = percent;
  const uint8_t pwmValue = static_cast<uint8_t>((static_cast<uint16_t>(percent) * 255U) / 100U);
  analogWrite(MOTOR_PWM_PIN, pwmValue);
}

uint8_t motorDriverGetSpeedPercent() {
  return g_speedPercent;
}

void motorDriverSetReverse(bool reversed) {
  g_reversed = reversed;
  digitalWrite(MOTOR_DIRECTION_PIN, g_reversed ? HIGH : LOW);
}

bool motorDriverIsReverse() {
  return g_reversed;
}

bool motorDriverTick() {
  return g_speedPercent > 0U;
}
