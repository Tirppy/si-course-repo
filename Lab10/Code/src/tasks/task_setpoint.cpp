#include "tasks/task_setpoint.h"

#include "app/app_state.h"
#include "services/setpoint_manager.h"

void taskSetpointRun() {
  const bool serialHandled = setpointManagerProcessSerial();

  if (!g_appState.control.serialOverrideEnabled) {
    g_appState.control.setpointTemperatureC = setpointManagerReadPotTemperature();
    g_appState.control.source = CommandSource::Potentiometer;
  } else if (serialHandled) {
    g_appState.control.source = CommandSource::Serial;
  }
}
