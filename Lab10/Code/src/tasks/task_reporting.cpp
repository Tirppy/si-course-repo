#include "tasks/task_reporting.h"

#include "app/app_state.h"
#include "drivers/lcd_display.h"
#include "drivers/serial_stdio.h"

namespace {

const char* commandSourceToString(CommandSource source) {
  switch (source) {
    case CommandSource::Serial:
      return "serial";
    case CommandSource::Potentiometer:
      return "pot";
    default:
      return "none";
  }
}

}  // namespace

void taskReportingRun() {
  if (!g_appState.periodicReportingEnabled && !g_appState.oneShotReportRequested) {
    return;
  }

  const bool heaterOn = g_appState.actuator.heaterAppliedState == ActuatorState::On;
  const uint8_t fanDutyPercent = g_appState.actuator.fanAppliedDutyPercent;

  if (!g_appState.sensor.valid) {
    lcdDisplayShowStatus(0.0F, g_appState.control.setpointTemperatureC,
                         g_appState.control.mode, false, 0U,
                         g_appState.control.serialOverrideEnabled);
    serialStdioPrintMessage("Invalid thermistor reading. Check divider wiring.");
    g_appState.oneShotReportRequested = false;
    return;
  }

  lcdDisplayShowStatus(g_appState.sensor.measuredTemperatureC,
                       g_appState.control.setpointTemperatureC,
                       g_appState.control.mode, heaterOn, fanDutyPercent,
                       g_appState.control.serialOverrideEnabled);

  serialStdioPrintStatus(
      g_appState.sensor.measuredTemperatureC, g_appState.control.setpointTemperatureC,
      g_appState.control.hysteresisWidthC, g_appState.control.hysteresisLowC,
      g_appState.control.hysteresisHighC,
      g_appState.control.acceptableMinTemperatureC,
      g_appState.control.acceptableMaxTemperatureC,
      g_appState.control.temperatureWithinRange, g_appState.control.mode, heaterOn,
      fanDutyPercent, g_appState.control.pidKp, g_appState.control.pidKi,
      g_appState.control.pidKd,
      g_appState.control.serialOverrideEnabled,
      commandSourceToString(g_appState.control.source));

  g_appState.oneShotReportRequested = false;
}
