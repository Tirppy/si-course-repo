#include "tasks/task_control.h"

#include "app/app_state.h"
#include "config/constants.h"
#include "services/on_off_controller.h"
#include "services/pid_controller.h"

void taskControlRun() {
  if (!g_appState.sensor.valid) {
    g_appState.actuator.heaterRequestedState = ActuatorState::Off;
    g_appState.actuator.fanRequestedDutyPercent = 0U;
    g_appState.control.pidOutputPercent = 0.0F;
    g_appState.control.signedOutputPercent = 0.0F;
    pidControllerReset(&g_appState.control.pidIntegral,
                       &g_appState.control.pidPreviousError);
    return;
  }

  const float dtSeconds = static_cast<float>(kControlPeriodMs) / 1000.0F;
  g_appState.control.errorC =
      g_appState.control.setpointTemperatureC - g_appState.sensor.measuredTemperatureC;

  g_appState.control.temperatureWithinRange =
      g_appState.sensor.measuredTemperatureC >= g_appState.control.acceptableMinTemperatureC &&
      g_appState.sensor.measuredTemperatureC <= g_appState.control.acceptableMaxTemperatureC;

  const ActuatorState onOffState = onOffControllerCompute(
      g_appState.sensor.measuredTemperatureC, g_appState.control.setpointTemperatureC,
      g_appState.control.hysteresisWidthC, g_appState.actuator.heaterRequestedState,
      &g_appState.control.hysteresisLowC, &g_appState.control.hysteresisHighC);

  float fanDutyPercent = 0.0F;
  ActuatorState heaterState = ActuatorState::Off;

  switch (g_appState.control.mode) {
    case ControlMode::Part1OnOff:
      heaterState = onOffState;
      pidControllerReset(&g_appState.control.pidIntegral,
                         &g_appState.control.pidPreviousError);
      break;

    case ControlMode::Part2Pid: {
      const float coolingError =
          g_appState.sensor.measuredTemperatureC - g_appState.control.setpointTemperatureC;
      if (coolingError > 0.0F) {
        fanDutyPercent = pidControllerComputeCoolingPercent(
            coolingError, dtSeconds, g_appState.control.pidKp, g_appState.control.pidKi,
            g_appState.control.pidKd, &g_appState.control.pidIntegral,
            &g_appState.control.pidPreviousError);
      } else {
        pidControllerReset(&g_appState.control.pidIntegral,
                           &g_appState.control.pidPreviousError);
      }
      break;
    }

    case ControlMode::Hybrid:
      heaterState = onOffState;

      if (heaterState == ActuatorState::On) {
        pidControllerReset(&g_appState.control.pidIntegral,
                           &g_appState.control.pidPreviousError);
      } else {
        const float coolingError =
            g_appState.sensor.measuredTemperatureC - g_appState.control.hysteresisHighC;
        if (coolingError > 0.0F) {
          fanDutyPercent = pidControllerComputeCoolingPercent(
              coolingError, dtSeconds, g_appState.control.pidKp,
              g_appState.control.pidKi, g_appState.control.pidKd,
              &g_appState.control.pidIntegral,
              &g_appState.control.pidPreviousError);
        } else {
          pidControllerReset(&g_appState.control.pidIntegral,
                             &g_appState.control.pidPreviousError);
        }
      }
      break;
  }

  g_appState.control.pidOutputPercent = fanDutyPercent;
  g_appState.actuator.heaterRequestedState = heaterState;
  g_appState.actuator.fanRequestedDutyPercent =
      static_cast<uint8_t>(fanDutyPercent + 0.5F);
  g_appState.control.signedOutputPercent =
      heaterState == ActuatorState::On ? 100.0F : -fanDutyPercent;
}
