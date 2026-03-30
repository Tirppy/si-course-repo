#include "services/analog_conditioning.h"

#include <Arduino.h>
#include <math.h>

#include "app/app_state.h"

namespace {

constexpr uint8_t MEDIAN_WINDOW = 5U;
constexpr uint8_t WEIGHTED_WINDOW = 4U;
constexpr float WEIGHTS[WEIGHTED_WINDOW] = {0.40F, 0.30F, 0.20F, 0.10F};
constexpr float MAX_RAMP_STEP_PERCENT = 8.0F;

float g_medianBuffer[MEDIAN_WINDOW] = {0.0F};
float g_weightedBuffer[WEIGHTED_WINDOW] = {0.0F};
bool g_medianPrimed = false;
bool g_weightedPrimed = false;

float clampPercent(float value) {
  if (value < 0.0F) {
    return 0.0F;
  }
  if (value > 100.0F) {
    return 100.0F;
  }
  return value;
}

void pushFront(float *buffer, uint8_t size, float value) {
  for (int index = size - 1; index > 0; --index) {
    buffer[index] = buffer[index - 1];
  }
  buffer[0] = value;
}

void primeBuffer(float *buffer, uint8_t size, float value) {
  for (uint8_t index = 0U; index < size; ++index) {
    buffer[index] = value;
  }
}

float medianOfFive(const float *buffer) {
  float sorted[MEDIAN_WINDOW];
  for (uint8_t index = 0U; index < MEDIAN_WINDOW; ++index) {
    sorted[index] = buffer[index];
  }

  for (uint8_t outer = 0U; outer < MEDIAN_WINDOW - 1U; ++outer) {
    for (uint8_t inner = outer + 1U; inner < MEDIAN_WINDOW; ++inner) {
      if (sorted[outer] > sorted[inner]) {
        const float temporary = sorted[outer];
        sorted[outer] = sorted[inner];
        sorted[inner] = temporary;
      }
    }
  }

  return sorted[MEDIAN_WINDOW / 2U];
}

float weightedAverage(const float *buffer) {
  float numerator = 0.0F;
  float denominator = 0.0F;

  for (uint8_t index = 0U; index < WEIGHTED_WINDOW; ++index) {
    numerator += buffer[index] * WEIGHTS[index];
    denominator += WEIGHTS[index];
  }

  return denominator > 0.0F ? (numerator / denominator) : buffer[0];
}

float applyRamp(float currentValue, float targetValue) {
  if ((targetValue - currentValue) > MAX_RAMP_STEP_PERCENT) {
    return currentValue + MAX_RAMP_STEP_PERCENT;
  }

  if ((currentValue - targetValue) > MAX_RAMP_STEP_PERCENT) {
    return currentValue - MAX_RAMP_STEP_PERCENT;
  }

  return targetValue;
}

uint8_t percentToDegrees(float percent) {
  const long roundedPercent = lroundf(percent);
  const long pwmValue = map(roundedPercent, 0L, 100L, 0L, 255L);
  return static_cast<uint8_t>(pwmValue);
}

}  // namespace

void analogConditioningInit() {
  g_appState.analog.rawPercent = 0.0F;
  g_appState.analog.saturatedPercent = 0.0F;
  g_appState.analog.medianPercent = 0.0F;
  g_appState.analog.weightedPercent = 0.0F;
  g_appState.analog.conditionedPercent = 0.0F;
  g_appState.analog.appliedPercent = 0.0F;
  g_appState.analog.pwmValue = 0U;
  g_appState.analog.saturationAlert = false;
  g_appState.analog.limitAlert = true;
  g_appState.analog.lastCommandAtMs = millis();
  g_appState.analog.lastActuatorAtMs = 0U;

  primeBuffer(g_medianBuffer, MEDIAN_WINDOW, 0.0F);
  primeBuffer(g_weightedBuffer, WEIGHTED_WINDOW, 0.0F);
  g_medianPrimed = true;
  g_weightedPrimed = true;
}

void analogConditioningSetRawPercent(float percent, unsigned long nowMs) {
  g_appState.analog.rawPercent = percent;
  g_appState.analog.lastCommandAtMs = nowMs;
}

void analogConditioningStep() {
  const float saturated = clampPercent(g_appState.analog.rawPercent);
  g_appState.analog.saturationAlert = (saturated != g_appState.analog.rawPercent);
  g_appState.analog.saturatedPercent = saturated;

  if (!g_medianPrimed) {
    primeBuffer(g_medianBuffer, MEDIAN_WINDOW, saturated);
    g_medianPrimed = true;
  }
  pushFront(g_medianBuffer, MEDIAN_WINDOW, saturated);
  g_appState.analog.medianPercent = medianOfFive(g_medianBuffer);

  if (!g_weightedPrimed) {
    primeBuffer(g_weightedBuffer, WEIGHTED_WINDOW, g_appState.analog.medianPercent);
    g_weightedPrimed = true;
  }
  pushFront(g_weightedBuffer, WEIGHTED_WINDOW, g_appState.analog.medianPercent);
  g_appState.analog.weightedPercent = weightedAverage(g_weightedBuffer);

  g_appState.analog.conditionedPercent =
      applyRamp(g_appState.analog.conditionedPercent, g_appState.analog.weightedPercent);
  g_appState.analog.limitAlert =
      (g_appState.analog.conditionedPercent <= 0.0F) ||
      (g_appState.analog.conditionedPercent >= 100.0F) ||
      g_appState.analog.saturationAlert;
  g_appState.analog.pwmValue = percentToDegrees(g_appState.analog.conditionedPercent);
}
