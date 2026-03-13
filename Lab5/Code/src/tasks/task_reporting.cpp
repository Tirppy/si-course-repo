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

uint32_t g_lastReportMs = 0U;

void formatFloat(float value, uint8_t width, uint8_t precision, char *buffer, size_t bufferSize) {
  if (isnan(value)) {
    snprintf(buffer, bufferSize, "n/a");
    return;
  }

  dtostrf(value, width, precision, buffer);
}

void printSensorBlock(const SensorRuntimeData &data, uint8_t confirmSamples) {
  char temperatureText[12] = {};

  if (!data.valid || isnan(data.temperatureC)) {
    printf("%s invalid alert=%s cnt=%u/%u\n",
           data.name,
           boolText(data.stableAlert),
           data.debounceCounter,
           confirmSamples);
    return;
  }

  formatFloat(data.temperatureC, 6, 2, temperatureText, sizeof(temperatureText));
  printf("%s %sC alert=%s cnt=%u/%u\n",
         data.name,
         temperatureText,
         boolText(data.stableAlert),
         data.debounceCounter,
         confirmSamples);
}
}  // namespace

void taskReportingRun() {
  const uint32_t nowMs = millis();
  if ((nowMs - g_lastReportMs) < REPORT_PERIOD_MS) {
    return;
  }
  g_lastReportMs = nowMs;

  char diffText[12] = {};
  char thresholdText[12] = {};
  char humidityText[12] = {};

  formatFloat(g_appState.temperatureDifferenceC, 5, 2, diffText, sizeof(diffText));
  formatFloat(g_appState.activeThresholdC, 5, 2, thresholdText, sizeof(thresholdText));
  formatFloat(g_appState.digitalHumidityPercent, 5, 1, humidityText, sizeof(humidityText));

  printf("\nTemps\n");

  printSensorBlock(g_appState.analog, ALERT_CONFIRM_SAMPLES);
  printSensorBlock(g_appState.digital, ALERT_CONFIRM_SAMPLES);

  printf("diff %sC buzzer=%s\n", diffText, boolText(g_appState.differenceAlert));
  printf("thr  %sC hum=%s%%\n", thresholdText, humidityText);
  fflush(stdout);

  ++g_appState.reportCycles;
}
