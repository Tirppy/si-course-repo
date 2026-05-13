#include "services/pid_controller.h"

#include "config/constants.h"

namespace {

float clampFloat(float value, float minValue, float maxValue) {
  if (value < minValue) {
    return minValue;
  }

  if (value > maxValue) {
    return maxValue;
  }

  return value;
}

}  // namespace

float pidControllerComputeCoolingPercent(float errorC,
                                         float dtSeconds,
                                         float kp,
                                         float ki,
                                         float kd,
                                         float* integral,
                                         float* previousError) {
  if (errorC <= 0.0F || dtSeconds <= 0.0F || integral == nullptr ||
      previousError == nullptr) {
    return 0.0F;
  }

  *integral += errorC * dtSeconds;
  *integral = clampFloat(*integral, kPidIntegralMin, kPidIntegralMax);

  const float derivative = (errorC - *previousError) / dtSeconds;
  const float output = (kp * errorC) + (ki * (*integral)) + (kd * derivative);

  *previousError = errorC;

  return clampFloat(output, kPidOutputMinPercent, kPidOutputMaxPercent);
}

void pidControllerReset(float* integral, float* previousError) {
  if (integral != nullptr) {
    *integral = 0.0F;
  }

  if (previousError != nullptr) {
    *previousError = 0.0F;
  }
}
