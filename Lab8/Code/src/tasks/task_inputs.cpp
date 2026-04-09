#include "tasks/task_inputs.h"

#include <Arduino.h>

#include "app/app_state.h"
#include "drivers/hardware_inputs.h"
#include "services/analog_conditioning.h"
#include "services/binary_conditioning.h"
#include "services/runtime_telemetry.h"

namespace {

bool g_lastButtonPressed = false;
uint8_t g_lastPotPercent = 0U;

bool hardwareEnabled() {
  return g_appState.inputMode != INPUT_MODE_SERIAL;
}

}  // namespace

void taskInputsSetup() {
  hardwareInputsInit();
  g_lastButtonPressed = hardwareInputsReadButtonPressed();
  g_lastPotPercent = hardwareInputsReadPotPercent();
}

void taskInputsRun() {
  if (!hardwareEnabled()) {
    return;
  }

  const bool buttonPressed = hardwareInputsReadButtonPressed();
  const uint8_t potPercent = hardwareInputsReadPotPercent();
  const unsigned long nowMs = millis();

  if (buttonPressed != g_lastButtonPressed) {
    g_lastButtonPressed = buttonPressed;
    binaryConditioningSetRawState(buttonPressed, nowMs);
    runtimeTelemetryLog(RUNTIME_EVENT_BINARY_CMD, buttonPressed ? 1 : 0, nowMs);
  }

  if (potPercent != g_lastPotPercent) {
    g_lastPotPercent = potPercent;
    analogConditioningSetRawPercent(static_cast<float>(potPercent), nowMs);
    runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_CMD, potPercent, nowMs);
  }
}
