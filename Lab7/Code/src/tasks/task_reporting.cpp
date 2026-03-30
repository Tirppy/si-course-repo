#include "tasks/task_reporting.h"

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "app/app_state.h"
#include "drivers/lcd_display.h"

namespace {

int toRoundedInt(float value) {
  return static_cast<int>(value + (value >= 0.0F ? 0.5F : -0.5F));
}

void printSnapshot(const char *reason) {
  const char *rawState = actuatorCommandToText(g_appState.binary.rawCommand);
  const char *conditionedState = actuatorCommandToText(g_appState.binary.conditionedCommand);
  const char *actuatorState = g_appState.binary.actuatorState ? "ON" : "OFF";
  char line1[17];
  char line2[17];
  const unsigned long binaryLatencyMs =
      (g_appState.binary.lastActuatorAtMs >= g_appState.binary.lastCommandAtMs)
          ? (g_appState.binary.lastActuatorAtMs - g_appState.binary.lastCommandAtMs)
          : 0UL;
  const unsigned long analogLatencyMs =
      (g_appState.analog.lastActuatorAtMs >= g_appState.analog.lastCommandAtMs)
          ? (g_appState.analog.lastActuatorAtMs - g_appState.analog.lastCommandAtMs)
          : 0UL;

  snprintf_P(line1, sizeof(line1), PSTR("B:%s A:%03d%%"), actuatorState,
           toRoundedInt(g_appState.analog.appliedPercent));
  snprintf_P(line2, sizeof(line2), PSTR("%s b%d a%d"),
           inputModeToText(g_appState.inputMode),
           g_appState.binary.alertPending ? 1 : 0,
           (g_appState.analog.saturationAlert || g_appState.analog.limitAlert) ? 1 : 0);

  lcdDisplayShowLines(line1, line2);

  printf_P(PSTR("REPORT reason=%s mode=%s binary_raw=%s binary_cond=%s binary_out=%s binary_alert=%u debounce_ms=%u binary_latency_ms=%lu analog_raw=%d analog_sat=%d analog_med=%d analog_avg=%d analog_ramp=%d analog_out=%d pwm=%u analog_alert=%u analog_latency_ms=%lu ok=%u err=%u tr=%u last=%s\n"),
         reason,
         inputModeToText(g_appState.inputMode),
         rawState,
         conditionedState,
         actuatorState,
         g_appState.binary.alertPending ? 1U : 0U,
         g_appState.binary.debounceWindowMs,
         binaryLatencyMs,
         toRoundedInt(g_appState.analog.rawPercent),
         toRoundedInt(g_appState.analog.saturatedPercent),
         toRoundedInt(g_appState.analog.medianPercent),
         toRoundedInt(g_appState.analog.weightedPercent),
         toRoundedInt(g_appState.analog.conditionedPercent),
         toRoundedInt(g_appState.analog.appliedPercent),
         g_appState.analog.pwmValue,
         (g_appState.analog.saturationAlert || g_appState.analog.limitAlert) ? 1U : 0U,
         analogLatencyMs,
         g_appState.acceptedCommandCount,
         g_appState.rejectedCommandCount,
         g_appState.transitionCount,
         g_appState.lastCommandText);

  if (g_appState.plotEnabled) {
    printf_P(PSTR("binary_raw:%u,binary_cond:%u,binary_out:%u,analog_raw:%d,analog_out:%d\n"),
           static_cast<unsigned>(g_appState.binary.rawCommand),
           static_cast<unsigned>(g_appState.binary.conditionedCommand),
           static_cast<unsigned>(g_appState.binary.actuatorState),
           toRoundedInt(g_appState.analog.rawPercent),
           toRoundedInt(g_appState.analog.appliedPercent));
  }
}

}  // namespace

void taskReportingSetup() {
  printSnapshot("startup");
}

void taskReportingRun() {
  if (!g_appState.autoReportEnabled) {
    return;
  }
  printSnapshot("periodic");
}

void taskReportingPrintImmediate(const char *reason) {
  printSnapshot(reason);
}
