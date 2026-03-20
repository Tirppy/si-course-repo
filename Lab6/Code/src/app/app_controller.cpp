#include "app_controller.h"

#include <Arduino.h>
#include <stdio.h>

#include "alert_led.h"
#include "analog_thermistor.h"
#include "app_config.h"
#include "app_state.h"
#include "digital_dht22_sensor.h"
#include "lcd_display.h"
#include "period_control.h"
#include "serial_stdio_bridge.h"
#include "task_acquisition.h"
#include "task_display.h"
#include "task_reporting.h"

namespace {
uint32_t g_lastAcquisitionRunMs = 0U;
uint32_t g_lastDisplayRunMs = 0U;
uint32_t g_lastReportRunMs = 0U;
}  // namespace

void appControllerInit() {
  serialStdioBridgeInit();
  analogThermistorInit();
  periodControlInit();
  digitalDht22SensorInit();
  alertLedInit();
  lcdDisplayInit();
  taskAcquisitionInit();

  printf("Lab 6 - Part 2 analog signal conditioning\n");
  printf("Variant C: thermistor + DHT22 on Arduino Uno\n");
  printf("Pipeline: saturation -> median -> weighted average -> alert\n");
}

void appControllerLoop() {
  const uint32_t nowMs = millis();

  if ((nowMs - g_lastAcquisitionRunMs) >= g_appState.acquisitionPeriodMs) {
    g_lastAcquisitionRunMs = nowMs;
    taskAcquisitionRun();
  }

  if ((nowMs - g_lastDisplayRunMs) >= DISPLAY_PERIOD_MS) {
    g_lastDisplayRunMs = nowMs;
    taskDisplayRun();
  }

  if ((nowMs - g_lastReportRunMs) >= g_appState.reportPeriodMs) {
    g_lastReportRunMs = nowMs;
    taskReportingRun();
  }
}
