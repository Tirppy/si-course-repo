#ifndef APP_APP_TYPES_H
#define APP_APP_TYPES_H

#include <Arduino.h>

enum class CommandSource {
  None,
  Potentiometer,
  Serial,
};

enum class ControlMode {
  Part1OnOff,
  Part2Pid,
  Hybrid,
};

enum class ActuatorState {
  Off,
  On,
};

struct SensorData {
  int adcValue;
  float voltageV;
  float resistanceOhms;
  float measuredTemperatureC;
  bool valid;
};

struct ControlData {
  float setpointTemperatureC;
  float hysteresisWidthC;
  float hysteresisLowC;
  float hysteresisHighC;
  float acceptableMinTemperatureC;
  float acceptableMaxTemperatureC;
  float errorC;
  float pidKp;
  float pidKi;
  float pidKd;
  float pidIntegral;
  float pidPreviousError;
  float pidOutputPercent;
  float signedOutputPercent;
  CommandSource source;
  ControlMode mode;
  bool serialOverrideEnabled;
  bool temperatureWithinRange;
};

struct ActuatorData {
  ActuatorState heaterRequestedState;
  ActuatorState heaterAppliedState;
  uint8_t fanRequestedDutyPercent;
  uint8_t fanAppliedDutyPercent;
};

struct AppState {
  SensorData sensor;
  ControlData control;
  ActuatorData actuator;
  bool periodicReportingEnabled;
  bool oneShotReportRequested;
};

#endif
