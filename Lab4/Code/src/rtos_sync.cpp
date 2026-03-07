#include "rtos_sync.h"

SemaphoreHandle_t g_pressEventSemaphore = nullptr;
SemaphoreHandle_t g_sharedDataMutex = nullptr;

bool rtosSyncInit() {
  g_pressEventSemaphore = xSemaphoreCreateBinary();
  g_sharedDataMutex = xSemaphoreCreateMutex();

  return g_pressEventSemaphore != nullptr && g_sharedDataMutex != nullptr;
}