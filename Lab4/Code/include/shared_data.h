#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <stdint.h>

struct PressEventData {
  bool isShortPress;
  uint32_t durationMs;
};

struct StatisticsData {
  uint32_t totalPresses;
  uint32_t shortPresses;
  uint32_t longPresses;
  uint32_t totalDurationMs;
};

extern PressEventData g_lastPressEvent;
extern StatisticsData g_statistics;

void resetSharedData();

#endif