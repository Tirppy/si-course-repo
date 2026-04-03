#include "drivers/motor_driver.h"

#include <Stepper.h>

namespace {

Stepper g_stepper(STEPPER_STEPS_PER_REVOLUTION,
                  STEPPER_IN1_PIN,
                  STEPPER_IN3_PIN,
                  STEPPER_IN2_PIN,
                  STEPPER_IN4_PIN);
uint8_t g_speedPercent = 0U;
unsigned long g_lastStepAtUs = 0UL;
int g_direction = 1;
bool g_reversed = false;

unsigned long percentToIntervalUs(uint8_t percent) {
  if (percent == 0U) {
    return 0UL;
  }

  const unsigned long spanUs = STEPPER_SLOWEST_INTERVAL_US - STEPPER_FASTEST_INTERVAL_US;
  const unsigned long scaledOffset = (spanUs * static_cast<unsigned long>(percent - 1U)) / 99UL;
  return STEPPER_SLOWEST_INTERVAL_US - scaledOffset;
}

}  // namespace

void motorDriverInit() {
  g_stepper.setSpeed(600);
  motorDriverSetSpeedPercent(0U);
  g_lastStepAtUs = micros();
}

void motorDriverSetSpeedPercent(uint8_t percent) {
  if (percent > 100U) {
    percent = 100U;
  }

  g_speedPercent = percent;
}

uint8_t motorDriverGetSpeedPercent() {
  return g_speedPercent;
}

void motorDriverSetReverse(bool reversed) {
  g_reversed = reversed;
  g_direction = g_reversed ? -1 : 1;
}

bool motorDriverIsReverse() {
  return g_reversed;
}

bool motorDriverTick() {
  if (g_speedPercent == 0U) {
    return false;
  }

  const unsigned long nowUs = micros();
  const unsigned long stepIntervalUs = percentToIntervalUs(g_speedPercent);
  if ((nowUs - g_lastStepAtUs) < stepIntervalUs) {
    return false;
  }

  g_lastStepAtUs = nowUs;
  g_stepper.step(g_direction);

  return true;
}
