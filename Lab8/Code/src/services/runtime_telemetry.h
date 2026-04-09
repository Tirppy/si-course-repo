#ifndef RUNTIME_TELEMETRY_H
#define RUNTIME_TELEMETRY_H

#include <Arduino.h>

enum RuntimeEventType : uint8_t {
  RUNTIME_EVENT_BINARY_CMD = 0,
  RUNTIME_EVENT_BINARY_OUT,
  RUNTIME_EVENT_ANALOG_CMD,
  RUNTIME_EVENT_ANALOG_OUT,
  RUNTIME_EVENT_INPUT_MODE,
  RUNTIME_EVENT_INVALID_CMD,
  RUNTIME_EVENT_HISTORY_CLEAR,
  RUNTIME_EVENT_STATS_RESET,
};

void runtimeTelemetryInit();
void runtimeTelemetryLog(RuntimeEventType type, int16_t value, unsigned long atMs);
void runtimeTelemetryRecordBinaryLatency(uint16_t latencyMs);
void runtimeTelemetryRecordAnalogLatency(uint16_t latencyMs);
void runtimeTelemetryPrintHistory();
void runtimeTelemetryClearHistory();
void runtimeTelemetryPrintStats();
void runtimeTelemetryResetStats();

#endif
