#include "app_controller.h"

#include <Arduino.h>
#include <stdio.h>

#include "alert_buzzer.h"
#include "alert_led.h"
#include "analog_ntc_sensor.h"
#include "app_config.h"
#include "digital_dht11_sensor.h"
#include "lcd_display.h"
#include "serial_stdio_bridge.h"
#include "task_acquisition.h"
#include "task_display.h"
#include "task_reporting.h"
#include "threshold_potentiometer.h"

namespace {
struct ScheduledTask {
  void (*run)();
  uint32_t periodMs;
  uint32_t lastRunMs;
};

ScheduledTask g_tasks[] = {
    {taskAcquisitionRun, ACQUISITION_PERIOD_MS, 0U},
    {taskDisplayRun, DISPLAY_PERIOD_MS, 0U},
    {taskReportingRun, REPORT_PERIOD_MS, 0U},
};

constexpr size_t TASK_COUNT = sizeof(g_tasks) / sizeof(g_tasks[0]);
}  // namespace

void appControllerInit() {
  serialStdioBridgeInit();
  analogNtcSensorInit();
  digitalDht11SensorInit();
  thresholdPotentiometerInit();
  alertLedInit();
  alertBuzzerInit();
  lcdDisplayInit();
  taskAcquisitionInit();

  printf("Lab 4 Variant C - dual temperature alert monitor\n");
  printf("Target: Arduino Uno + Wokwi + PlatformIO\n");
}

void appControllerLoop() {
  const uint32_t nowMs = millis();

  for (size_t index = 0; index < TASK_COUNT; ++index) {
    ScheduledTask &task = g_tasks[index];
    if ((nowMs - task.lastRunMs) >= task.periodMs) {
      task.lastRunMs = nowMs;
      task.run();
    }
  }
}
