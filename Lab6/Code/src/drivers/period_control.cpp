#include "period_control.h"

#include <Arduino.h>

#include "app_config.h"

void periodControlInit() {
  pinMode(PERIOD_CONTROL_PIN, INPUT);
}

uint16_t periodControlReadRaw() {
  return static_cast<uint16_t>(analogRead(PERIOD_CONTROL_PIN));
}

uint32_t mapPotentiometerToAcquisitionPeriodMs(uint16_t rawAdc) {
  const uint32_t constrained = rawAdc > ADC_MAX_VALUE ? ADC_MAX_VALUE : rawAdc;
  return ACQUISITION_PERIOD_MIN_MS +
         ((ACQUISITION_PERIOD_MAX_MS - ACQUISITION_PERIOD_MIN_MS) * constrained) / ADC_MAX_VALUE;
}

uint32_t mapAcquisitionToReportPeriodMs(uint32_t acquisitionPeriodMs) {
  uint32_t reportPeriodMs = acquisitionPeriodMs * REPORT_PERIOD_FACTOR;
  if (reportPeriodMs < REPORT_PERIOD_MIN_MS) {
    reportPeriodMs = REPORT_PERIOD_MIN_MS;
  }
  if (reportPeriodMs > REPORT_PERIOD_MAX_MS) {
    reportPeriodMs = REPORT_PERIOD_MAX_MS;
  }
  return reportPeriodMs;
}
