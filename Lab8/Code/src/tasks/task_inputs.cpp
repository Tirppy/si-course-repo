#include "tasks/task_inputs.h"

#include <Arduino.h>

#include "app/app_state.h"
#include "drivers/hardware_inputs.h"
#include "services/analog_conditioning.h"
#include "services/binary_conditioning.h"
#include "services/runtime_telemetry.h"

namespace {

bool g_lastButtonPressed = false;

bool hardwareEnabled() {
  return g_appState.inputMode != INPUT_MODE_SERIAL;
}

}  // namespace

void taskInputsSetup() {
  hardwareInputsInit();
  g_lastButtonPressed = hardwareInputsReadButtonPressed();
}

void taskInputsRun() {
  if (!hardwareEnabled()) {
    return;
  }

  const bool buttonPressed = hardwareInputsReadButtonPressed();
  const unsigned long nowMs = millis();

  if (buttonPressed != g_lastButtonPressed) {
    g_lastButtonPressed = buttonPressed;
    binaryConditioningSetRawState(buttonPressed, nowMs);
    runtimeTelemetryLog(RUNTIME_EVENT_BINARY_CMD, buttonPressed ? 1 : 0, nowMs);
  }

  (void)nowMs;
}
