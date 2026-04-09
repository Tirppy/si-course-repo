#include "services/runtime_telemetry.h"

#include <avr/pgmspace.h>
#include <stdio.h>

namespace {

constexpr uint8_t HISTORY_CAPACITY = 8U;

struct HistoryEntry {
  unsigned long atMs;
  int16_t value;
  uint8_t type;
};

struct LatencyStats {
  uint16_t lastMs;
  uint16_t minMs;
  uint16_t maxMs;
  uint32_t totalMs;
  uint16_t samples;
};

HistoryEntry g_history[HISTORY_CAPACITY];
uint8_t g_historyHead = 0U;
uint8_t g_historyCount = 0U;
LatencyStats g_binaryStats = {0U, 0U, 0U, 0UL, 0U};
LatencyStats g_analogStats = {0U, 0U, 0U, 0UL, 0U};

const char *eventTypeToText(uint8_t type) {
  switch (type) {
    case RUNTIME_EVENT_BINARY_CMD:
      return "BIN_CMD";
    case RUNTIME_EVENT_BINARY_OUT:
      return "BIN_OUT";
    case RUNTIME_EVENT_ANALOG_CMD:
      return "STP_CMD";
    case RUNTIME_EVENT_ANALOG_OUT:
      return "STP_OUT";
    case RUNTIME_EVENT_INPUT_MODE:
      return "INPUT";
    case RUNTIME_EVENT_INVALID_CMD:
      return "INVALID";
    case RUNTIME_EVENT_HISTORY_CLEAR:
      return "HIST_CLR";
    case RUNTIME_EVENT_STATS_RESET:
      return "STAT_RST";
    default:
      return "EVENT";
  }
}

void resetStats(LatencyStats &stats) {
  stats.lastMs = 0U;
  stats.minMs = 0U;
  stats.maxMs = 0U;
  stats.totalMs = 0UL;
  stats.samples = 0U;
}

void recordStats(LatencyStats &stats, uint16_t latencyMs) {
  stats.lastMs = latencyMs;
  if (stats.samples == 0U || latencyMs < stats.minMs) {
    stats.minMs = latencyMs;
  }
  if (stats.samples == 0U || latencyMs > stats.maxMs) {
    stats.maxMs = latencyMs;
  }
  stats.totalMs += latencyMs;
  ++stats.samples;
}

void printStatsLine(const char *label, const LatencyStats &stats) {
  const unsigned long averageMs = stats.samples > 0U ? (stats.totalMs / stats.samples) : 0UL;
  printf_P(PSTR("%s last=%u min=%u max=%u avg=%lu n=%u\n"),
           label,
           stats.lastMs,
           stats.minMs,
           stats.maxMs,
           averageMs,
           stats.samples);
}

}  // namespace

void runtimeTelemetryInit() {
  g_historyHead = 0U;
  g_historyCount = 0U;
  resetStats(g_binaryStats);
  resetStats(g_analogStats);
}

void runtimeTelemetryLog(RuntimeEventType type, int16_t value, unsigned long atMs) {
  g_history[g_historyHead].atMs = atMs;
  g_history[g_historyHead].value = value;
  g_history[g_historyHead].type = type;
  g_historyHead = static_cast<uint8_t>((g_historyHead + 1U) % HISTORY_CAPACITY);
  if (g_historyCount < HISTORY_CAPACITY) {
    ++g_historyCount;
  }
}

void runtimeTelemetryRecordBinaryLatency(uint16_t latencyMs) {
  recordStats(g_binaryStats, latencyMs);
}

void runtimeTelemetryRecordAnalogLatency(uint16_t latencyMs) {
  recordStats(g_analogStats, latencyMs);
}

void runtimeTelemetryPrintHistory() {
  printf_P(PSTR("HISTORY count=%u\n"), g_historyCount);
  for (uint8_t index = 0U; index < g_historyCount; ++index) {
    const uint8_t entryIndex =
        static_cast<uint8_t>((g_historyHead + HISTORY_CAPACITY - g_historyCount + index) % HISTORY_CAPACITY);
    printf_P(PSTR("%lu %s %d\n"),
             g_history[entryIndex].atMs,
             eventTypeToText(g_history[entryIndex].type),
             g_history[entryIndex].value);
  }
}

void runtimeTelemetryClearHistory() {
  g_historyHead = 0U;
  g_historyCount = 0U;
}

void runtimeTelemetryPrintStats() {
  printf_P(PSTR("STATS\n"));
  printStatsLine("BINARY", g_binaryStats);
  printStatsLine("STEPPER", g_analogStats);
}

void runtimeTelemetryResetStats() {
  resetStats(g_binaryStats);
  resetStats(g_analogStats);
}
