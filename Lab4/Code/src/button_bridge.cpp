#include "button_bridge.h"

#include "button.h"

namespace {
constexpr uint32_t BUTTON_DEBOUNCE_MS = 25U;

bool g_stablePressed = false;
bool g_candidatePressed = false;
uint32_t g_candidateSinceMs = 0U;
}

void buttonBridgeInit() {
  g_stablePressed = buttonIsPressed();
  g_candidatePressed = g_stablePressed;
  g_candidateSinceMs = 0U;
}

void buttonBridgeUpdate(uint32_t nowMs) {
  const bool rawPressed = buttonIsPressed();

  if (rawPressed != g_candidatePressed) {
    g_candidatePressed = rawPressed;
    g_candidateSinceMs = nowMs;
    return;
  }

  if (g_candidatePressed != g_stablePressed && (nowMs - g_candidateSinceMs) >= BUTTON_DEBOUNCE_MS) {
    g_stablePressed = g_candidatePressed;
  }
}

bool buttonBridgeIsPressed() {
  return g_stablePressed;
}