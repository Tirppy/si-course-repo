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
  g_appState.control.pidKp = kPidKpDefault;
  g_appState.control.pidKi = kPidKiDefault;
  g_appState.control.pidKd = kPidKdDefault;
  g_appState.control.pidIntegral = 0.0F;
  g_appState.control.pidPreviousError = 0.0F;
  g_appState.control.pidOutputPercent = 0.0F;
  g_appState.control.signedOutputPercent = 0.0F;
  g_appState.control.source = CommandSource::Potentiometer;
  g_appState.control.mode = ControlMode::Hybrid;
  g_appState.control.serialOverrideEnabled = false;
  g_appState.control.temperatureWithinRange = true;
  g_appState.actuator.heaterRequestedState = ActuatorState::Off;
  g_appState.actuator.heaterAppliedState = ActuatorState::Off;
  g_appState.actuator.fanRequestedDutyPercent = 0U;
  g_appState.actuator.fanAppliedDutyPercent = 0U;
  g_appState.periodicReportingEnabled = true;
  g_appState.oneShotReportRequested = false;
}
