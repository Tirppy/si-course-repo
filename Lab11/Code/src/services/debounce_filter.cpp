#include "debounce_filter.h"

void debounceFilterInit(DebounceFilter &filter,
                        bool initialValue,
                        uint8_t requiredSamples) {
  filter.stableValue = initialValue;
  filter.candidateValue = initialValue;
  filter.counter = 0U;
  filter.requiredSamples = requiredSamples == 0U ? 1U : requiredSamples;
}

bool debounceFilterUpdate(DebounceFilter &filter, bool rawValue) {
  if (rawValue == filter.stableValue) {
    filter.candidateValue = rawValue;
    filter.counter = 0U;
    return false;
  }

  if (rawValue != filter.candidateValue) {
    filter.candidateValue = rawValue;
    filter.counter = 1U;
    return false;
  }

  if (filter.counter < filter.requiredSamples) {
    filter.counter++;
  }

  if (filter.counter < filter.requiredSamples) {
    return false;
  }

  filter.stableValue = filter.candidateValue;
  filter.counter = 0U;
  return true;
}

bool debounceFilterStableValue(const DebounceFilter &filter) {
  return filter.stableValue;
}
