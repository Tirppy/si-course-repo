#include "task1.h"

#include "button_bridge.h"
#include "leds.h"
#include "platform_time.h"
#include "shared_data.h"

namespace {
constexpr uint32_t SHORT_PRESS_THRESHOLD_MS = 500U;
constexpr uint32_t DEBOUNCE_MIN_PRESS_MS = 30U;
constexpr uint32_t SHORT_LONG_LED_HOLD_MS = 250U;

bool g_previousPressed = false;
uint32_t g_pressStartMs = 0U;
uint32_t g_indicatorOffAtMs = 0U;
}  // namespace

void task1_buttonDetectAndMeasure() {
  const uint32_t now = platformMillis();
  buttonBridgeUpdate(now);
  const bool isPressed = buttonBridgeIsPressed();

  if (isPressed && !g_previousPressed) {
    g_pressStartMs = now;
  }

  if (!isPressed && g_previousPressed) {
    const uint32_t durationMs = now - g_pressStartMs;
    if (durationMs >= DEBOUNCE_MIN_PRESS_MS) {
      const bool isShortPress = durationMs < SHORT_PRESS_THRESHOLD_MS;

      g_pressEvent.durationMs = durationMs;
      g_pressEvent.isShortPress = isShortPress;
      g_pressEvent.hasNewPress = true;

      ledsShortLongOff();
      if (isShortPress) {
        ledsGreenOn();
      } else {
        ledsRedOn();
      }
      g_indicatorOffAtMs = now + SHORT_LONG_LED_HOLD_MS;
    }
  }

  g_previousPressed = isPressed;

  if (g_indicatorOffAtMs != 0U && now >= g_indicatorOffAtMs) {
    ledsShortLongOff();
    g_indicatorOffAtMs = 0U;
  }
}