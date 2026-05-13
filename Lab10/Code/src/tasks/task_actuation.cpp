#include "tasks/task_actuation.h"

#include "app/app_state.h"
#include "drivers/fan_motor_driver.h"
#include "drivers/relay_actuator.h"

void taskActuationRun() {
  g_appState.actuator.heaterAppliedState = g_appState.actuator.heaterRequestedState;
  g_appState.actuator.fanAppliedDutyPercent = g_appState.actuator.fanRequestedDutyPercent;

  heaterOutputApply(g_appState.actuator.heaterAppliedState);
  fanMotorSetDutyPercent(g_appState.actuator.fanAppliedDutyPercent);
}
