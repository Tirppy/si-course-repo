#ifndef APP_STATE_H
#define APP_STATE_H

#include <Arduino.h>

enum ActuatorCommand : uint8_t {
  ACTUATOR_COMMAND_OFF = 0,
  ACTUATOR_COMMAND_ON = 1,
};

enum InputMode : uint8_t {
  INPUT_MODE_SERIAL = 0,
  INPUT_MODE_HARDWARE = 1,
  INPUT_MODE_HYBRID = 2,
};

struct BinaryActuatorState {
  ActuatorCommand rawCommand;
  ActuatorCommand conditionedCommand;
  bool actuatorState;
  bool alertPending;
  uint16_t debounceWindowMs;
  uint16_t lastDebounceLatencyMs;
  unsigned long lastCommandAtMs;
  unsigned long lastActuatorAtMs;
};

struct AnalogActuatorState {
  float rawPercent;
  float saturatedPercent;
  float medianPercent;
  float weightedPercent;
  float conditionedPercent;
  float appliedPercent;
  uint8_t pwmValue;
  bool saturationAlert;
  bool limitAlert;
  unsigned long lastCommandAtMs;
  unsigned long lastActuatorAtMs;
};

struct AppRuntimeState {
  BinaryActuatorState binary;
  AnalogActuatorState analog;
  uint16_t acceptedCommandCount;
  uint16_t rejectedCommandCount;
  uint16_t transitionCount;
  InputMode inputMode;
  bool plotEnabled;
  bool autoReportEnabled;
  char lastCommandText[16];
};

extern AppRuntimeState g_appState;

const char *actuatorCommandToText(ActuatorCommand command);
const char *inputModeToText(InputMode mode);

#endif
