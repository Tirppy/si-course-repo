#include "tasks/task_actuator.h"

#include <Arduino.h>

#include "app/app_state.h"
#include "drivers/actuator_led.h"
#include "drivers/analog_servo.h"
#include "services/runtime_telemetry.h"

void taskActuatorSetup() {
  g_appState.binary.actuatorState = actuatorLedGetState();
  g_appState.analog.pwmValue = analogServoGetDegrees();
}

void taskActuatorRun() {
  const bool requestedBinaryState =
      (g_appState.binary.conditionedCommand == ACTUATOR_COMMAND_ON);

  if (requestedBinaryState != g_appState.binary.actuatorState) {
    actuatorLedSetState(requestedBinaryState);
    g_appState.binary.actuatorState = requestedBinaryState;
    g_appState.binary.lastActuatorAtMs = millis();
    ++g_appState.transitionCount;
    runtimeTelemetryLog(RUNTIME_EVENT_BINARY_OUT, requestedBinaryState ? 1 : 0, g_appState.binary.lastActuatorAtMs);
    runtimeTelemetryRecordBinaryLatency(
        static_cast<uint16_t>(g_appState.binary.lastActuatorAtMs - g_appState.binary.lastCommandAtMs));
  }

  const uint8_t requestedPwm = g_appState.analog.pwmValue;
  if (requestedPwm != analogServoGetDegrees()) {
    analogServoWriteDegrees(requestedPwm);
    g_appState.analog.appliedPercent = g_appState.analog.conditionedPercent;
    g_appState.analog.lastActuatorAtMs = millis();
    ++g_appState.transitionCount;
    runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_OUT,
                        static_cast<int16_t>(g_appState.analog.appliedPercent),
                        g_appState.analog.lastActuatorAtMs);
    runtimeTelemetryRecordAnalogLatency(
        static_cast<uint16_t>(g_appState.analog.lastActuatorAtMs - g_appState.analog.lastCommandAtMs));
  }
}
