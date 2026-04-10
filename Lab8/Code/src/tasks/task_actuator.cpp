#include "tasks/task_actuator.h"

#include <Arduino.h>
#include <math.h>

#include "app/app_state.h"
#include "drivers/indicator_led.h"
#include "drivers/motor_driver.h"
#include "services/binary_conditioning.h"
#include "services/runtime_telemetry.h"

void taskActuatorSetup() {
  g_appState.analog.pwmValue = motorDriverGetSpeedPercent();
  g_appState.led.appliedState = indicatorLedGetState();
  g_appState.binary.actuatorState = actuatorGetState();
}

void taskActuatorRun() {
  if (g_appState.binary.conditionedState != g_appState.binary.actuatorState) {
    indicatorLedSetState(g_appState.binary.conditionedState);
    g_appState.binary.actuatorState = g_appState.binary.conditionedState;
    g_appState.binary.lastActuatorAtMs = millis();
    g_appState.led.appliedState = g_appState.binary.actuatorState;
    ++g_appState.transitionCount;
    runtimeTelemetryLog(RUNTIME_EVENT_BINARY_OUT, g_appState.binary.actuatorState ? 1 : 0, g_appState.binary.lastActuatorAtMs);
    runtimeTelemetryRecordBinaryLatency(
        static_cast<uint16_t>(g_appState.binary.lastActuatorAtMs - g_appState.binary.lastCommandAtMs));
  }

  const uint8_t requestedPercent =
      static_cast<uint8_t>(lroundf(g_appState.analog.conditionedPercent));
  if (requestedPercent != motorDriverGetSpeedPercent()) {
    motorDriverSetSpeedPercent(requestedPercent);
    g_appState.analog.appliedPercent = g_appState.analog.conditionedPercent;
    g_appState.analog.lastActuatorAtMs = millis();
    ++g_appState.transitionCount;
    runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_OUT,
                        static_cast<int16_t>(g_appState.analog.appliedPercent),
                        g_appState.analog.lastActuatorAtMs);
    runtimeTelemetryRecordAnalogLatency(
        static_cast<uint16_t>(g_appState.analog.lastActuatorAtMs - g_appState.analog.lastCommandAtMs));
  }

  const bool shouldReverse = g_appState.binary.actuatorState && (motorDriverGetSpeedPercent() >= 1U);
  if (shouldReverse != motorDriverIsReverse()) {
    motorDriverSetReverse(shouldReverse);
  }

  if (motorDriverTick()) {
    g_appState.analog.appliedPercent = g_appState.analog.conditionedPercent;
  }
}
