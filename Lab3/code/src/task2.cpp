#include "task2.h"

#include "leds.h"
#include "platform_time.h"
#include "shared_data.h"

namespace {
constexpr uint8_t SHORT_PRESS_BLINKS = 5U;
constexpr uint8_t LONG_PRESS_BLINKS = 10U;
constexpr uint32_t YELLOW_TOGGLE_INTERVAL_MS = 70U;

uint16_t g_pendingYellowToggles = 0U;
bool g_yellowState = false;
uint32_t g_lastYellowToggleMs = 0U;

void queueYellowBlinks(uint8_t blinkCount) {
  g_pendingYellowToggles += static_cast<uint16_t>(blinkCount) * 2U;
}
}  // namespace

void task2_statisticsAndBlink() {
  const uint32_t now = platformMillis();

  if (g_pressEvent.hasNewPress) {
    g_pressEvent.hasNewPress = false;

    g_statistics.totalPresses++;
    g_statistics.totalDurationMs += g_pressEvent.durationMs;

    if (g_pressEvent.isShortPress) {
      g_statistics.shortPresses++;
      queueYellowBlinks(SHORT_PRESS_BLINKS);
    } else {
      g_statistics.longPresses++;
      queueYellowBlinks(LONG_PRESS_BLINKS);
    }
  }

  if (g_pendingYellowToggles > 0U && (now - g_lastYellowToggleMs) >= YELLOW_TOGGLE_INTERVAL_MS) {
    g_lastYellowToggleMs = now;
    g_yellowState = !g_yellowState;

    if (g_yellowState) {
      ledsYellowOn();
    } else {
      ledsYellowOff();
    }

    g_pendingYellowToggles--;
  }

  if (g_pendingYellowToggles == 0U && g_yellowState) {
    g_yellowState = false;
    ledsYellowOff();
  }
}