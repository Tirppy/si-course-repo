#include "task2.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "leds.h"
#include "rtos_sync.h"
#include "shared_data.h"

namespace {
constexpr uint8_t SHORT_PRESS_BLINKS = 5U;
constexpr uint8_t LONG_PRESS_BLINKS = 10U;
constexpr TickType_t YELLOW_TOGGLE_INTERVAL = pdMS_TO_TICKS(70U);

constexpr TickType_t TASK2_PERIOD = pdMS_TO_TICKS(20U);
constexpr TickType_t TASK2_OFFSET = pdMS_TO_TICKS(5U);

uint16_t g_pendingYellowToggles = 0U;
bool g_yellowState = false;
TickType_t g_lastYellowToggleTick = 0U;

void queueYellowBlinks(uint8_t blinkCount) {
  g_pendingYellowToggles += static_cast<uint16_t>(blinkCount) * 2U;
}
}

void task2_statisticsAndBlink(void* parameters) {
  (void)parameters;

  if (TASK2_OFFSET > 0U) {
    vTaskDelay(TASK2_OFFSET);
  }

  TickType_t lastWakeTick = xTaskGetTickCount();
  while (true) {
    if (xSemaphoreTake(g_pressEventSemaphore, 0U) == pdTRUE) {
      PressEventData localEvent{};
      bool eventReady = false;

      if (xSemaphoreTake(g_sharedDataMutex, pdMS_TO_TICKS(10U)) == pdTRUE) {
        localEvent = g_lastPressEvent;

        g_statistics.totalPresses++;
        g_statistics.totalDurationMs += localEvent.durationMs;
        if (localEvent.isShortPress) {
          g_statistics.shortPresses++;
        } else {
          g_statistics.longPresses++;
        }

        xSemaphoreGive(g_sharedDataMutex);
        eventReady = true;
      }

      if (eventReady) {
        if (localEvent.isShortPress) {
          queueYellowBlinks(SHORT_PRESS_BLINKS);
        } else {
          queueYellowBlinks(LONG_PRESS_BLINKS);
        }
      }
    }

    const TickType_t nowTick = xTaskGetTickCount();
    if (g_pendingYellowToggles > 0U && (nowTick - g_lastYellowToggleTick) >= YELLOW_TOGGLE_INTERVAL) {
      g_lastYellowToggleTick = nowTick;
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

    vTaskDelayUntil(&lastWakeTick, TASK2_PERIOD);
  }
}