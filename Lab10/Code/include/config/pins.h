#ifndef CONFIG_PINS_H
#define CONFIG_PINS_H

#include <Arduino.h>

constexpr uint8_t kThermistorPin = A0;
constexpr uint8_t kSetpointPotPin = A1;
constexpr uint8_t kHeaterOutputPin = 8;
constexpr uint8_t kMotorDriverIn1Pin = 2;
constexpr uint8_t kMotorDriverIn2Pin = 10;

constexpr uint8_t kLcdRsPin = 7;
constexpr uint8_t kLcdEnablePin = 6;
constexpr uint8_t kLcdD4Pin = 5;
constexpr uint8_t kLcdD5Pin = 4;
constexpr uint8_t kLcdD6Pin = 3;
constexpr uint8_t kLcdD7Pin = 13;

#endif
