#include "tasks/task_control.h"

#include "app/app_state.h"
#include "config/constants.h"
#include "services/on_off_controller.h"

void taskControlRun() {
  if (!g_appState.sensor.valid) {
    g_appState.actuator.requestedState = ActuatorState::Off;
    return;
  }

  g_appState.control.errorC =
      g_appState.control.setpointTemperatureC - g_appState.sensor.measuredTemperatureC;

  g_appState.control.temperatureWithinRange =
      g_appState.sensor.measuredTemperatureC >= g_appState.control.acceptableMinTemperatureC &&
      g_appState.sensor.measuredTemperatureC <= g_appState.control.acceptableMaxTemperatureC;

  g_appState.actuator.requestedState = onOffControllerCompute(
      g_appState.sensor.measuredTemperatureC, g_appState.control.setpointTemperatureC,
      g_appState.control.hysteresisWidthC, g_appState.actuator.requestedState,
      &g_appState.control.hysteresisLowC, &g_appState.control.hysteresisHighC);
}
