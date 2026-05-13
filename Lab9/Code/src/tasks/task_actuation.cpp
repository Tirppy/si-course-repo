#include "tasks/task_actuation.h"

#include "app/app_state.h"
#include "drivers/relay_actuator.h"

void taskActuationRun() {
  g_appState.actuator.appliedState = g_appState.actuator.requestedState;
  actuatorOutputApply(g_appState.actuator.appliedState);
}
