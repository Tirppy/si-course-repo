#ifndef RTOS_SYNC_H
#define RTOS_SYNC_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

extern SemaphoreHandle_t g_pressEventSemaphore;
extern SemaphoreHandle_t g_sharedDataMutex;

bool rtosSyncInit();

#endif