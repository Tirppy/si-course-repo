#include "services/on_off_controller.h"

ActuatorState onOffControllerCompute(float measuredTemperatureC,
                                     float setpointTemperatureC,
                                     float hysteresisWidthC,
                                     ActuatorState previousState,
                                     float* lowThresholdC,
                                     float* highThresholdC) {
  const float halfBand = hysteresisWidthC * 0.5F;
  const float lowThreshold = setpointTemperatureC - halfBand;
  const float highThreshold = setpointTemperatureC + halfBand;

  if (lowThresholdC != nullptr) {
    *lowThresholdC = lowThreshold;
  }

  if (highThresholdC != nullptr) {
    *highThresholdC = highThreshold;
  }

  if (measuredTemperatureC < lowThreshold) {
    return ActuatorState::On;
  }

  if (measuredTemperatureC > highThreshold) {
    return ActuatorState::Off;
  }

  return previousState;
}
