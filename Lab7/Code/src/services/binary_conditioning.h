#ifndef BINARY_CONDITIONING_H
#define BINARY_CONDITIONING_H

#include <Arduino.h>

void binaryConditioningInit(uint16_t debounceWindowMs, uint16_t persistSamples);
void binaryConditioningSetRawState(bool requestedState, unsigned long nowMs);
void binaryConditioningSetDebounceWindow(uint16_t debounceWindowMs);
void binaryConditioningUpdate(unsigned long nowMs);
bool actuatorGetState();

#endif
