#include "tasks/task_reporting.h"

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "app/app_state.h"
#include "drivers/hardware_inputs.h"
#include "drivers/lcd_display.h"

namespace {

int toRoundedInt(float value) {
  return static_cast<int>(value + (value >= 0.0F ? 0.5F : -0.5F));
}

void printSnapshot(const char *reason) {
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

  snprintf_P(line1, sizeof(line1), PSTR("B:%s A:%03d%%"),
             g_appState.led.appliedState ? "ON" : "OFF",
             toRoundedInt(g_appState.analog.appliedPercent));
  snprintf_P(line2, sizeof(line2), PSTR("%s b%d a%d"),
             inputModeToText(g_appState.inputMode),
             hardwareInputsReadButtonPressed() ? 1 : 0,
             (g_appState.analog.saturationAlert || g_appState.analog.limitAlert) ? 1 : 0);

  lcdDisplayShowLines(line1, line2);

  printf_P(PSTR("REPORT reason=%s mode=%s led=%u stepper_raw=%d stepper_sat=%d stepper_med=%d stepper_avg=%d stepper_ramp=%d stepper_out=%d pos_pct=%u stepper_alert=%u stepper_latency_ms=%lu ok=%u err=%u tr=%u last=%s button=%u\n"),
           reason,
           inputModeToText(g_appState.inputMode),
           g_appState.led.appliedState ? 1U : 0U,
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
           g_appState.lastCommandText,
           hardwareInputsReadButtonPressed() ? 1U : 0U);

  printf_P(PSTR("BINARY raw=%u sat=%u cand=%u cond=%u out=%u pending=%u debounce_ms=%u persist=%u count=%u latency_ms=%lu\n"),
           static_cast<unsigned>(g_appState.binary.rawState),
           static_cast<unsigned>(g_appState.binary.saturatedState),
           static_cast<unsigned>(g_appState.binary.candidateState),
           static_cast<unsigned>(g_appState.binary.conditionedState),
           static_cast<unsigned>(g_appState.binary.actuatorState),
           g_appState.binary.alertPending ? 1U : 0U,
           g_appState.binary.debounceWindowMs,
           g_appState.binary.persistSamples,
           g_appState.binary.confirmationCounter,
           binaryLatencyMs);

  if (g_appState.plotEnabled) {
    printf_P(PSTR("led:%u,stepper_raw:%d,stepper_out:%d,button:%u\n"),
             g_appState.led.appliedState ? 1U : 0U,
             toRoundedInt(g_appState.analog.rawPercent),
             toRoundedInt(g_appState.analog.appliedPercent),
             hardwareInputsReadButtonPressed() ? 1U : 0U);
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
