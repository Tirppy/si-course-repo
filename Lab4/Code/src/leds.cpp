#include "leds.h"

#include <Arduino.h>

namespace {
constexpr uint8_t LED_GREEN_PIN = 9U;
constexpr uint8_t LED_RED_PIN = 10U;
constexpr uint8_t LED_YELLOW_PIN = 11U;
}

void ledsInit() {
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);

  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
}

void ledsGreenOn() {
  digitalWrite(LED_GREEN_PIN, HIGH);
}

void ledsGreenOff() {
  digitalWrite(LED_GREEN_PIN, LOW);
}

void ledsRedOn() {
  digitalWrite(LED_RED_PIN, HIGH);
}

void ledsRedOff() {
  digitalWrite(LED_RED_PIN, LOW);
}

void ledsYellowOn() {
  digitalWrite(LED_YELLOW_PIN, HIGH);
}

void ledsYellowOff() {
  digitalWrite(LED_YELLOW_PIN, LOW);
}

void ledsShortLongOff() {
  ledsGreenOff();
  ledsRedOff();
}