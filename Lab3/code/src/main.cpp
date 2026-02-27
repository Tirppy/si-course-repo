#include <stdio.h>

#include "button.h"
#include "button_bridge.h"
#include "leds.h"
#include "platform_time.h"
#include "scheduler.h"
#include "shared_data.h"
#include "stdio_bridge.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

namespace {
TaskContext g_tasks[] = {
    {task1_buttonDetectAndMeasure, 20U, 0U, 0U, true, "Task1"},
    {task2_statisticsAndBlink, 20U, 5U, 0U, true, "Task2"},
    {task3_periodicReport, 10000U, 10U, 0U, true, "Task3"},
};

uint32_t g_lastTickMs = 0U;
}  // namespace

extern "C" void setup() {
  platformTimeInit();
  stdioBridgeInit(9600);
  buttonInit();
  buttonBridgeInit();
  ledsInit();
  resetSharedData();

  schedulerInit(g_tasks, sizeof(g_tasks) / sizeof(g_tasks[0]));
  g_lastTickMs = platformMillis();

  printf("Lab 3 - Part 1 (non-preemptive bare-metal scheduler)\n");
  printf("Button monitoring started.\n\n");
}

extern "C" void loop() {
  const uint32_t now = platformMillis();
  if (now != g_lastTickMs) {
    g_lastTickMs = now;
    schedulerTick(now);
  }
}