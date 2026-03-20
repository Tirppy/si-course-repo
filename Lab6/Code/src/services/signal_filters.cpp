#include "signal_filters.h"

#include <Arduino.h>

namespace {
float medianOfBuffer(const float *values, uint8_t count) {
  float scratch[5] = {};

  for (uint8_t index = 0; index < count; ++index) {
    scratch[index] = values[index];
  }

  for (uint8_t left = 0; left < count; ++left) {
    for (uint8_t right = left + 1; right < count; ++right) {
      if (scratch[right] < scratch[left]) {
        const float temporary = scratch[left];
        scratch[left] = scratch[right];
        scratch[right] = temporary;
      }
    }
  }

  return scratch[count / 2U];
}
}  // namespace

void medianFilterReset(MedianFilterState &state) {
  for (uint8_t index = 0; index < 5U; ++index) {
    state.buffer[index] = 0.0F;
  }
  state.count = 0U;
  state.nextIndex = 0U;
}

float medianFilterApply(MedianFilterState &state, float sample) {
  state.buffer[state.nextIndex] = sample;
  state.nextIndex = (state.nextIndex + 1U) % 5U;
  if (state.count < 5U) {
    ++state.count;
  }

  return medianOfBuffer(state.buffer, state.count);
}

void weightedAverageReset(WeightedAverageState &state) {
  for (uint8_t index = 0; index < 4U; ++index) {
    state.buffer[index] = 0.0F;
  }
  state.count = 0U;
  state.nextIndex = 0U;
}

float weightedAverageApply(WeightedAverageState &state,
                           float sample,
                           const float *weights,
                           uint8_t weightCount) {
  state.buffer[state.nextIndex] = sample;
  state.nextIndex = (state.nextIndex + 1U) % weightCount;
  if (state.count < weightCount) {
    ++state.count;
  }

  float weightedSum = 0.0F;
  float totalWeight = 0.0F;

  for (uint8_t index = 0; index < state.count; ++index) {
    const int16_t bufferIndex = static_cast<int16_t>(state.nextIndex) - 1 - static_cast<int16_t>(index);
    const uint8_t wrappedIndex =
        static_cast<uint8_t>((bufferIndex + static_cast<int16_t>(weightCount)) % weightCount);

    weightedSum += state.buffer[wrappedIndex] * weights[index];
    totalWeight += weights[index];
  }

  return totalWeight > 0.0F ? (weightedSum / totalWeight) : sample;
}

float saturateFloat(float value, float minValue, float maxValue) {
  if (value < minValue) {
    return minValue;
  }
  if (value > maxValue) {
    return maxValue;
  }
  return value;
}
