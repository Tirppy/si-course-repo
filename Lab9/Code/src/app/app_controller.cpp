#include "app/app_controller.h"

#include <Arduino.h>

#include "app/app_state.h"
#include "config/constants.h"
#include "drivers/lcd_display.h"
#include "drivers/relay_actuator.h"
#include "drivers/serial_stdio.h"
#include "drivers/setpoint_input.h"
#include "drivers/thermistor_sensor.h"
#include "tasks/task_acquisition.h"
#include "tasks/task_actuation.h"
#include "tasks/task_control.h"
#include "tasks/task_reporting.h"
#include "tasks/task_setpoint.h"

namespace {

struct ScheduledTask {
  void (*handler)();
  unsigned long periodMs;
  unsigned long lastRunMs;
};

ScheduledTask g_tasks[] = {
    {taskAcquisitionRun, kAcquisitionPeriodMs, 0UL},
    {taskSetpointRun, kSetpointPeriodMs, 0UL},
    {taskControlRun, kControlPeriodMs, 0UL},
    {taskActuationRun, kActuationPeriodMs, 0UL},
    {taskReportingRun, kReportingPeriodMs, 0UL},
};

}  // namespace

void appInit() {
  appStateReset();
  serialStdioInit(kSerialBaudRate);
  thermistorSensorInit();
  setpointInputInit();
  actuatorOutputInit();
  lcdDisplayInit();
  serialStdioPrintHelp();
}

void appRun() {
  const unsigned long nowMs = millis();

  for (ScheduledTask& task : g_tasks) {
    if ((nowMs - task.lastRunMs) >= task.periodMs) {
      task.lastRunMs = nowMs;
      task.handler();
    }
  }
}
