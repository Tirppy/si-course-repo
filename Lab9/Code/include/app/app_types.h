#ifndef APP_APP_TYPES_H
#define APP_APP_TYPES_H

enum class CommandSource {
  None,
  Potentiometer,
  Serial,
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
  CommandSource source;
  bool serialOverrideEnabled;
  bool temperatureWithinRange;
};

struct ActuatorData {
  ActuatorState requestedState;
  ActuatorState appliedState;
};

struct AppState {
  SensorData sensor;
  ControlData control;
  ActuatorData actuator;
  bool periodicReportingEnabled;
  bool oneShotReportRequested;
};

#endif
