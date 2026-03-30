#include "services/command_conditioning.h"

#include "app/app_state.h"

namespace {

ActuatorCommand g_candidateCommand = ACTUATOR_COMMAND_OFF;
unsigned long g_candidateSinceMs = 0U;

}  // namespace

void commandConditioningInit(uint16_t debounceWindowMs) {
  g_appState.binary.rawCommand = ACTUATOR_COMMAND_OFF;
  g_appState.binary.conditionedCommand = ACTUATOR_COMMAND_OFF;
  g_appState.binary.debounceWindowMs = debounceWindowMs;
  g_appState.binary.alertPending = false;
  g_candidateCommand = ACTUATOR_COMMAND_OFF;
  g_candidateSinceMs = millis();
}

void commandConditioningSetRawCommand(ActuatorCommand command, unsigned long nowMs) {
  g_appState.binary.rawCommand = command;
  g_appState.binary.lastCommandAtMs = nowMs;
  if (g_candidateCommand != command) {
    g_candidateCommand = command;
    g_candidateSinceMs = nowMs;
  }
}

void commandConditioningSetDebounceWindow(uint16_t debounceWindowMs) {
  g_appState.binary.debounceWindowMs = debounceWindowMs;
}

void commandConditioningUpdate(unsigned long nowMs) {
  if (g_candidateCommand != g_appState.binary.rawCommand) {
    g_candidateCommand = g_appState.binary.rawCommand;
    g_candidateSinceMs = nowMs;
  }

  g_appState.binary.alertPending = (g_candidateCommand != g_appState.binary.conditionedCommand);

  if (g_candidateCommand == g_appState.binary.conditionedCommand) {
    return;
  }

  if ((nowMs - g_candidateSinceMs) >= g_appState.binary.debounceWindowMs) {
    g_appState.binary.conditionedCommand = g_candidateCommand;
    g_appState.binary.lastDebounceLatencyMs = nowMs - g_candidateSinceMs;
    g_appState.binary.alertPending = false;
  }
}
