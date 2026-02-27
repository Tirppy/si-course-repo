#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include <stdint.h>

typedef void (*TaskFunction)();

struct TaskContext {
  TaskFunction taskFunction;
  uint16_t periodMs;
  uint16_t offsetMs;
  uint16_t countdownMs;
  bool enabled;
  const char* taskName;
};

void schedulerInit(TaskContext* tasks, size_t taskCount);
void schedulerTick(uint32_t nowMs);

#endif