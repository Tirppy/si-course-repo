#include "drivers/motor_driver.h"

namespace {

uint8_t g_speedPercent = 0U;
bool g_reversed = false;

}  // namespace

void motorDriverInit() {
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  analogWrite(MOTOR_ENABLE_PIN, 0);
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  motorDriverSetSpeedPercent(0U);
}

void motorDriverSetSpeedPercent(uint8_t percent) {
  if (percent > 100U) {
    percent = 100U;
  }

  g_speedPercent = percent;
  const uint8_t pwmValue = static_cast<uint8_t>((static_cast<uint16_t>(percent) * 255U) / 100U);
  analogWrite(MOTOR_ENABLE_PIN, pwmValue);
}

uint8_t motorDriverGetSpeedPercent() {
  return g_speedPercent;
}

void motorDriverSetReverse(bool reversed) {
  g_reversed = reversed;

  if (g_speedPercent == 0U) {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    return;
  }

  if (g_reversed) {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_IN1_PIN, HIGH);
    digitalWrite(MOTOR_IN2_PIN, LOW);
  }
}

bool motorDriverIsReverse() {
  return g_reversed;
}

bool motorDriverTick() {
  if (g_speedPercent == 0U) {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    return false;
  }

  motorDriverSetReverse(g_reversed);
  return g_speedPercent > 0U;
}
