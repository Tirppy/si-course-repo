#include "scheduler.h"

namespace {
TaskContext* g_tasks = nullptr;
size_t g_taskCount = 0U;
}

void schedulerInit(TaskContext* tasks, size_t taskCount) {
  g_tasks = tasks;
  g_taskCount = taskCount;

  for (size_t index = 0U; index < g_taskCount; ++index) {
    g_tasks[index].countdownMs = g_tasks[index].offsetMs;
  }
}

void schedulerTick(uint32_t nowMs) {
  (void)nowMs;

  for (size_t index = 0U; index < g_taskCount; ++index) {
    if (g_tasks[index].enabled && g_tasks[index].countdownMs > 0U) {
      --g_tasks[index].countdownMs;
    }
  }

  for (size_t index = 0U; index < g_taskCount; ++index) {
    TaskContext& task = g_tasks[index];
    if (!task.enabled || task.countdownMs != 0U) {
      continue;
    }

    task.countdownMs = task.periodMs;
    task.taskFunction();
    return;
  }
}