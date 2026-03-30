#include "tasks/task_conditioning.h"

#include <Arduino.h>

#include "services/analog_conditioning.h"
#include "services/command_conditioning.h"

void taskConditioningSetup() {
}

void taskConditioningRun() {
  commandConditioningUpdate(millis());
  analogConditioningStep();
}
