#include "tasks/task_conditioning.h"

#include <Arduino.h>

#include "services/analog_conditioning.h"
#include "services/binary_conditioning.h"

void taskConditioningSetup() {
}

void taskConditioningRun() {
  binaryConditioningUpdate(millis());
  analogConditioningStep();
}
