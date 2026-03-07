#include "shared_data.h"

PressEventData g_lastPressEvent = {true, 0U};
StatisticsData g_statistics = {0U, 0U, 0U, 0U};

void resetSharedData() {
  g_lastPressEvent = {true, 0U};
  g_statistics = {0U, 0U, 0U, 0U};
}