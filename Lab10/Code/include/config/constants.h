#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H

#include <Arduino.h>

constexpr unsigned long kSerialBaudRate = 115200UL;

constexpr unsigned long kAcquisitionPeriodMs = 150UL;
constexpr unsigned long kSetpointPeriodMs = 120UL;
constexpr unsigned long kControlPeriodMs = 150UL;
constexpr unsigned long kActuationPeriodMs = 150UL;
constexpr unsigned long kReportingPeriodMs = 500UL;

constexpr float kSetpointMinC = 18.0F;
constexpr float kSetpointMaxC = 45.0F;
constexpr float kDefaultSetpointC = 26.0F;
constexpr float kHysteresisWidthC = 1.4F;
constexpr float kHysteresisMinC = 0.2F;
constexpr float kHysteresisMaxC = 10.0F;
constexpr float kAcceptableMinDefaultC = 15.0F;
constexpr float kAcceptableMaxDefaultC = 50.0F;

constexpr float kSeriesResistorOhms = 10000.0F;
constexpr float kNominalResistanceOhms = 10000.0F;
constexpr float kNominalTemperatureC = 25.0F;
constexpr float kBetaCoefficient = 3950.0F;
constexpr float kKelvinOffset = 273.15F;

constexpr int kAdcMaxValue = 1023;
constexpr float kSupplyVoltage = 5.0F;

constexpr float kSerialSetpointStepC = 0.5F;

constexpr float kPidKpDefault = 35.0F;
constexpr float kPidKiDefault = 0.8F;
constexpr float kPidKdDefault = 12.0F;
constexpr float kPidOutputMinPercent = 0.0F;
constexpr float kPidOutputMaxPercent = 100.0F;
constexpr float kPidIntegralMin = 0.0F;
constexpr float kPidIntegralMax = 100.0F;

constexpr unsigned long kMotorSoftPwmPeriodUs = 20000UL;

#endif
