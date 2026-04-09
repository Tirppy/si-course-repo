#ifndef ANALOG_CONDITIONING_H
#define ANALOG_CONDITIONING_H

#include <Arduino.h>

void analogConditioningInit();
void analogConditioningSetRawPercent(float percent, unsigned long nowMs);
void analogConditioningStep();

#endif
