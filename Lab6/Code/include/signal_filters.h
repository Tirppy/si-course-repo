#ifndef SIGNAL_FILTERS_H
#define SIGNAL_FILTERS_H

#include <stdint.h>

#include "models.h"

void medianFilterReset(MedianFilterState &state);
float medianFilterApply(MedianFilterState &state, float sample);

void weightedAverageReset(WeightedAverageState &state);
float weightedAverageApply(WeightedAverageState &state,
                           float sample,
                           const float *weights,
                           uint8_t weightCount);

float saturateFloat(float value, float minValue, float maxValue);

#endif
