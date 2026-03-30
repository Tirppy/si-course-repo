#ifndef COMMAND_CONDITIONING_H
#define COMMAND_CONDITIONING_H

#include <Arduino.h>

#include "app/app_state.h"

void commandConditioningInit(uint16_t debounceWindowMs);
void commandConditioningSetRawCommand(ActuatorCommand command, unsigned long nowMs);
void commandConditioningSetDebounceWindow(uint16_t debounceWindowMs);
void commandConditioningUpdate(unsigned long nowMs);

#endif
