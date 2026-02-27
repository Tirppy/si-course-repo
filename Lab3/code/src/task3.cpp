#include "task3.h"

#include <stdio.h>

#include "shared_data.h"
#include "stdio_bridge.h"

void task3_periodicReport() {
  uint32_t averageDurationIntMs = 0U;
  uint32_t averageDurationTenthMs = 0U;
  if (g_statistics.totalPresses > 0U) {
    averageDurationIntMs = g_statistics.totalDurationMs / g_statistics.totalPresses;
    averageDurationTenthMs =
        ((g_statistics.totalDurationMs * 10U) / g_statistics.totalPresses) % 10U;
  }

    printf("[10s report]\n");
    printf("Total presses: %lu\n", static_cast<unsigned long>(g_statistics.totalPresses));
    printf("Short presses (<500ms): %lu\n", static_cast<unsigned long>(g_statistics.shortPresses));
    printf("Long presses (>=500ms): %lu\n", static_cast<unsigned long>(g_statistics.longPresses));
    printf("Average duration: %lu.%lu ms\n\n",
      static_cast<unsigned long>(averageDurationIntMs),
      static_cast<unsigned long>(averageDurationTenthMs));

  g_statistics = {0U, 0U, 0U, 0U};
}