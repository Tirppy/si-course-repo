#include <Arduino_FreeRTOS.h>

#include <stdio.h>

#include "button.h"
#include "button_bridge.h"
#include "leds.h"
#include "rtos_sync.h"
#include "shared_data.h"
#include "stdio_bridge.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

namespace {
constexpr uint32_t SERIAL_BAUDRATE = 9600U;
constexpr uint16_t TASK_STACK_WORDS = 192U;
constexpr UBaseType_t PRIORITY_TASK1 = 3U;
constexpr UBaseType_t PRIORITY_TASK2 = 2U;
constexpr UBaseType_t PRIORITY_TASK3 = 1U;
}  // namespace

extern "C" void setup() {
  stdioBridgeInit(SERIAL_BAUDRATE);
  buttonInit();
  buttonBridgeInit();
  ledsInit();
  resetSharedData();

  if (!rtosSyncInit()) {
    while (true) {
      ledsYellowOn();
      vTaskDelay(pdMS_TO_TICKS(150));
      ledsYellowOff();
      vTaskDelay(pdMS_TO_TICKS(150));
    }
  }

  xTaskCreate(task1_buttonDetectAndMeasure, "Task1", TASK_STACK_WORDS, nullptr, PRIORITY_TASK1,
              nullptr);
  xTaskCreate(task2_statisticsAndBlink, "Task2", TASK_STACK_WORDS, nullptr, PRIORITY_TASK2,
              nullptr);
  xTaskCreate(task3_periodicReport, "Task3", TASK_STACK_WORDS, nullptr, PRIORITY_TASK3,
              nullptr);

  printf("Lab 4 - Part 2 (FreeRTOS preemptive tasks)\n");
  printf("Button monitoring started.\n\n");
}

extern "C" void loop() {
}