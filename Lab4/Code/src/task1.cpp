#include "task1.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "button_bridge.h"
#include "leds.h"
#include "rtos_sync.h"
#include "shared_data.h"

namespace {
constexpr uint32_t SHORT_PRESS_THRESHOLD_MS = 500U;
constexpr uint32_t MIN_VALID_PRESS_MS = 30U;
constexpr uint32_t SHORT_LONG_LED_HOLD_MS = 250U;

constexpr TickType_t TASK1_PERIOD = pdMS_TO_TICKS(20U);
constexpr TickType_t TASK1_OFFSET = pdMS_TO_TICKS(0U);

bool g_previousPressed = false;
TickType_t g_pressStartTick = 0U;
TickType_t g_indicatorOffTick = 0U;
}

void task1_buttonDetectAndMeasure(void* parameters) {
  (void)parameters;

  if (TASK1_OFFSET > 0U) {
    vTaskDelay(TASK1_OFFSET);
  }

  TickType_t lastWakeTick = xTaskGetTickCount();
  while (true) {
    const TickType_t nowTick = xTaskGetTickCount();
    const uint32_t nowMs = static_cast<uint32_t>(nowTick) * portTICK_PERIOD_MS;

    buttonBridgeUpdate(nowMs);
    const bool isPressed = buttonBridgeIsPressed();

    if (isPressed && !g_previousPressed) {
      g_pressStartTick = nowTick;
    }

    if (!isPressed && g_previousPressed) {
      const uint32_t durationMs =
          static_cast<uint32_t>(nowTick - g_pressStartTick) * portTICK_PERIOD_MS;

      if (durationMs >= MIN_VALID_PRESS_MS) {
        const bool isShortPress = durationMs < SHORT_PRESS_THRESHOLD_MS;

        if (xSemaphoreTake(g_sharedDataMutex, pdMS_TO_TICKS(10U)) == pdTRUE) {
          g_lastPressEvent.durationMs = durationMs;
          g_lastPressEvent.isShortPress = isShortPress;
          xSemaphoreGive(g_sharedDataMutex);
          xSemaphoreGive(g_pressEventSemaphore);
        }

        ledsShortLongOff();
        if (isShortPress) {
          ledsGreenOn();
        } else {
          ledsRedOn();
        }

        g_indicatorOffTick = nowTick + pdMS_TO_TICKS(SHORT_LONG_LED_HOLD_MS);
      }
    }

    g_previousPressed = isPressed;

    if (g_indicatorOffTick != 0U && nowTick >= g_indicatorOffTick) {
      ledsShortLongOff();
      g_indicatorOffTick = 0U;
    }

    vTaskDelayUntil(&lastWakeTick, TASK1_PERIOD);
  }
}