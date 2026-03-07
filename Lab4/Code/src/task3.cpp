#include "task3.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>

#include "rtos_sync.h"
#include "shared_data.h"

namespace {
constexpr TickType_t REPORT_PERIOD = pdMS_TO_TICKS(10000U);
constexpr TickType_t TASK3_OFFSET = pdMS_TO_TICKS(10U);
}

void task3_periodicReport(void* parameters) {
  (void)parameters;

  if (TASK3_OFFSET > 0U) {
    vTaskDelay(TASK3_OFFSET);
  }

  TickType_t lastWakeTick = xTaskGetTickCount();
  while (true) {
    StatisticsData snapshot = {0U, 0U, 0U, 0U};
    bool hasSnapshot = false;

    if (xSemaphoreTake(g_sharedDataMutex, pdMS_TO_TICKS(20U)) == pdTRUE) {
      snapshot = g_statistics;
      g_statistics = {0U, 0U, 0U, 0U};
      xSemaphoreGive(g_sharedDataMutex);
      hasSnapshot = true;
    }

    if (hasSnapshot) {
      uint32_t averageDurationIntMs = 0U;
      uint32_t averageDurationTenthMs = 0U;
      if (snapshot.totalPresses > 0U) {
        averageDurationIntMs = snapshot.totalDurationMs / snapshot.totalPresses;
        averageDurationTenthMs = ((snapshot.totalDurationMs * 10U) / snapshot.totalPresses) % 10U;
      }

      printf("[10s report][FreeRTOS]\n");
      printf("Total presses: %lu\n", static_cast<unsigned long>(snapshot.totalPresses));
      printf("Short presses (<500ms): %lu\n", static_cast<unsigned long>(snapshot.shortPresses));
      printf("Long presses (>=500ms): %lu\n", static_cast<unsigned long>(snapshot.longPresses));
      printf("Average duration: %lu.%lu ms\n\n",
             static_cast<unsigned long>(averageDurationIntMs),
             static_cast<unsigned long>(averageDurationTenthMs));
    }

    vTaskDelayUntil(&lastWakeTick, REPORT_PERIOD);
  }
}