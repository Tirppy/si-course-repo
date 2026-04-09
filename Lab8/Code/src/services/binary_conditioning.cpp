#include "services/binary_conditioning.h"

#include "app/app_state.h"

namespace {

bool g_candidateState = false;
unsigned long g_candidateSinceMs = 0U;
uint16_t g_confirmationCounter = 0U;

}  // namespace

void binaryConditioningInit(uint16_t debounceWindowMs, uint16_t persistSamples) {
  g_appState.binary.rawState = false;
  g_appState.binary.saturatedState = false;
  g_appState.binary.candidateState = false;
  g_appState.binary.conditionedState = false;
  g_appState.binary.actuatorState = false;
  g_appState.binary.alertPending = false;
  g_appState.binary.debounceWindowMs = debounceWindowMs;
  g_appState.binary.persistSamples = persistSamples;
  g_appState.binary.confirmationCounter = 0U;
  g_appState.binary.lastCommandAtMs = millis();
  g_appState.binary.lastActuatorAtMs = 0U;
  g_candidateState = false;
  g_candidateSinceMs = millis();
  g_confirmationCounter = 0U;
}

void binaryConditioningSetRawState(bool requestedState, unsigned long nowMs) {
  g_appState.binary.rawState = requestedState;
  g_appState.binary.saturatedState = requestedState;
  g_appState.binary.lastCommandAtMs = nowMs;
}

void binaryConditioningSetDebounceWindow(uint16_t debounceWindowMs) {
  g_appState.binary.debounceWindowMs = debounceWindowMs;
}

void binaryConditioningUpdate(unsigned long nowMs) {
  const bool saturatedState = g_appState.binary.saturatedState;

  if (saturatedState != g_candidateState) {
    g_candidateState = saturatedState;
    g_candidateSinceMs = nowMs;
    g_confirmationCounter = 0U;
  }

  g_appState.binary.candidateState = g_candidateState;
  g_appState.binary.alertPending = (g_candidateState != g_appState.binary.conditionedState);

  if (g_candidateState == g_appState.binary.conditionedState) {
    g_confirmationCounter = 0U;
    g_appState.binary.confirmationCounter = 0U;
    return;
  }

  if ((nowMs - g_candidateSinceMs) < g_appState.binary.debounceWindowMs) {
    return;
  }

  if (g_confirmationCounter < g_appState.binary.persistSamples) {
    ++g_confirmationCounter;
  }
  g_appState.binary.confirmationCounter = g_confirmationCounter;

  if (g_confirmationCounter >= g_appState.binary.persistSamples) {
    g_appState.binary.conditionedState = g_candidateState;
    g_appState.binary.alertPending = false;
    g_confirmationCounter = 0U;
    g_appState.binary.confirmationCounter = 0U;
  }
}

bool actuatorGetState() {
  return g_appState.binary.actuatorState;
}
