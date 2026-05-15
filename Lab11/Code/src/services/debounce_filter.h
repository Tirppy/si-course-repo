#ifndef DEBOUNCE_FILTER_H
#define DEBOUNCE_FILTER_H

#include <stdint.h>

struct DebounceFilter {
  bool stableValue;
  bool candidateValue;
  uint8_t counter;
  uint8_t requiredSamples;
};

void debounceFilterInit(DebounceFilter &filter,
                        bool initialValue,
                        uint8_t requiredSamples);
bool debounceFilterUpdate(DebounceFilter &filter, bool rawValue);
bool debounceFilterStableValue(const DebounceFilter &filter);

#endif
