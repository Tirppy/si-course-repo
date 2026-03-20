#include "task_reporting.h"

#include <Arduino.h>
#include <math.h>
#include <stdio.h>

#include "app_config.h"
#include "app_state.h"

namespace {
const char *boolText(bool value) {
  return value ? "ON" : "OFF";
}

const char *trendText(int8_t direction) {
  if (direction > 0) {
    return "UP";
  }
  if (direction < 0) {
    return "DOWN";
  }
  return "STABLE";
}

void formatFloat(float value, uint8_t width, uint8_t precision, char *buffer, size_t bufferSize) {
  if (isnan(value)) {
    snprintf(buffer, bufferSize, "n/a");
    return;
  }

  dtostrf(value, width, precision, buffer);
}

void printSensorBlock(const SensorRuntimeData &data) {
  char rawText[12] = {};
  char satText[12] = {};
  char medText[12] = {};
  char avgText[12] = {};
  char tempText[12] = {};
  char minText[12] = {};
  char maxText[12] = {};
  char deltaText[12] = {};

  formatFloat(data.rawValue, 6, 2, rawText, sizeof(rawText));
  formatFloat(data.saturatedValue, 6, 2, satText, sizeof(satText));
  formatFloat(data.medianValue, 6, 2, medText, sizeof(medText));
  formatFloat(data.weightedValue, 6, 2, avgText, sizeof(avgText));
  formatFloat(data.temperatureC, 6, 2, tempText, sizeof(tempText));
  formatFloat(data.minimumTemperatureC, 6, 2, minText, sizeof(minText));
  formatFloat(data.maximumTemperatureC, 6, 2, maxText, sizeof(maxText));
  formatFloat(data.lastDeltaTemperatureC, 6, 2, deltaText, sizeof(deltaText));

  printf("%s raw=%s sat=%s med=%s avg=%s\n", data.name, rawText, satText, medText, avgText);
  printf("  temp=%sC min=%sC max=%sC dT=%sC\n", tempText, minText, maxText, deltaText);
  printf("  trend=%s fast=%s cand=%s alert=%s cnt=%u/%u valid=%s\n",
         trendText(data.trendDirection),
         boolText(data.fastChangeDetected),
         boolText(data.alertCandidate),
         boolText(data.stableAlert),
         data.alertCounter,
         ALERT_CONFIRM_SAMPLES,
         boolText(data.valid));
}
}  // namespace

void taskReportingRun() {
  printf("\nSignal report\n");
  printf("cfg thr=%0.1fC acq=%lums rpt=%lums ctrl=%u noise=%s fastFault=%s\n",
         static_cast<double>(ALERT_THRESHOLD_C),
         static_cast<unsigned long>(g_appState.acquisitionPeriodMs),
         static_cast<unsigned long>(g_appState.reportPeriodMs),
         g_appState.periodControlRaw,
         boolText(g_appState.noiseInjectionEnabled),
         boolText(g_appState.fastChangeFaultActive));
  printSensorBlock(g_appState.analog);
  printSensorBlock(g_appState.digital);
  fflush(stdout);

  ++g_appState.reportCycles;
}
