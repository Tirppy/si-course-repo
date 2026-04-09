#include "app/app_controller.h"

#include <Arduino.h>
#include <string.h>

#include "app/app_state.h"
#include "drivers/motor_driver.h"
#include "drivers/lcd_display.h"
#include "drivers/indicator_led.h"
#include "drivers/serial_stdio.h"
#include "services/analog_conditioning.h"
#include "services/binary_conditioning.h"
#include "services/runtime_telemetry.h"
#include "tasks/task_actuator.h"
#include "tasks/task_command.h"
#include "tasks/task_conditioning.h"
#include "tasks/task_inputs.h"
#include "tasks/task_reporting.h"

namespace {

struct ScheduledTask {
  void (*callback)();
  uint32_t periodMs;
  unsigned long lastRunMs;
};

constexpr uint32_t COMMAND_TASK_PERIOD_MS = 20U;
constexpr uint32_t INPUTS_TASK_PERIOD_MS = 20U;
constexpr uint32_t CONDITIONING_TASK_PERIOD_MS = 20U;
constexpr uint32_t ACTUATOR_TASK_PERIOD_MS = 20U;
constexpr uint32_t REPORTING_TASK_PERIOD_MS = 500U;
constexpr uint16_t DEFAULT_BINARY_DEBOUNCE_MS = 60U;
constexpr uint16_t DEFAULT_BINARY_PERSIST_SAMPLES = 2U;

ScheduledTask g_tasks[] = {
    {taskCommandRun, COMMAND_TASK_PERIOD_MS, 0U},
    {taskInputsRun, INPUTS_TASK_PERIOD_MS, 0U},
    {taskConditioningRun, CONDITIONING_TASK_PERIOD_MS, 0U},
    {taskActuatorRun, ACTUATOR_TASK_PERIOD_MS, 0U},
    {taskReportingRun, REPORTING_TASK_PERIOD_MS, 0U},
};

}  // namespace

AppRuntimeState g_appState = {
    {false, false, false, false, false, false, DEFAULT_BINARY_DEBOUNCE_MS, DEFAULT_BINARY_PERSIST_SAMPLES, 0U, 0U, 0U},
    {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0U, false, true, 0U, 0U},
    {false, false},
    0U,
    0U,
    0U,
    INPUT_MODE_HYBRID,
    false,
    true,
    "BOOT",
};

const char *inputModeToText(InputMode mode) {
  switch (mode) {
    case INPUT_MODE_SERIAL:
      return "SERIAL";
    case INPUT_MODE_HARDWARE:
      return "HARDWARE";
    case INPUT_MODE_HYBRID:
    default:
      return "HYBRID";
  }
}

void appControllerSetup() {
  motorDriverInit();
  indicatorLedInit();
  lcdDisplayInit();
  serialStdioInit(115200UL);
  binaryConditioningInit(DEFAULT_BINARY_DEBOUNCE_MS, DEFAULT_BINARY_PERSIST_SAMPLES);
  analogConditioningInit();
  runtimeTelemetryInit();

  taskCommandSetup();
  taskInputsSetup();
  taskConditioningSetup();
  taskActuatorSetup();
  taskReportingSetup();

  printf_P(PSTR("Lab 7 binary+analog ready\n"));
  printf_P(PSTR("Type HELP for commands\n"));
}

void appControllerLoop() {
  const unsigned long nowMs = millis();

  for (ScheduledTask &task : g_tasks) {
    if ((nowMs - task.lastRunMs) >= task.periodMs) {
      task.lastRunMs = nowMs;
      task.callback();
    }
  }
}
