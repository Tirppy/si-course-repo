#include "app/app_state.h"

#include "config/constants.h"

AppState g_appState;

void appStateReset() {
  g_appState.sensor = {0, 0.0F, 0.0F, 0.0F, false};
  g_appState.control.setpointTemperatureC = kDefaultSetpointC;
  g_appState.control.hysteresisWidthC = kHysteresisWidthC;
  g_appState.control.hysteresisLowC = kDefaultSetpointC - (kHysteresisWidthC * 0.5F);
  g_appState.control.hysteresisHighC = kDefaultSetpointC + (kHysteresisWidthC * 0.5F);
  g_appState.control.acceptableMinTemperatureC = kAcceptableMinDefaultC;
  g_appState.control.acceptableMaxTemperatureC = kAcceptableMaxDefaultC;
  g_appState.control.errorC = 0.0F;
  g_appState.control.source = CommandSource::Potentiometer;
  g_appState.control.serialOverrideEnabled = false;
  g_appState.control.temperatureWithinRange = true;
  g_appState.actuator.requestedState = ActuatorState::Off;
  g_appState.actuator.appliedState = ActuatorState::Off;
  g_appState.periodicReportingEnabled = true;
  g_appState.oneShotReportRequested = false;
}
