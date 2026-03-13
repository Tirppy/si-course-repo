#include "alert_buzzer.h"

#include <Arduino.h>

#include "app_config.h"

namespace {
constexpr uint16_t BUZZER_TONE_HZ = 1760U;
}

void alertBuzzerInit() {
  pinMode(ALERT_BUZZER_PIN, OUTPUT);
  noTone(ALERT_BUZZER_PIN);
}

void alertBuzzerSet(bool active) {
  if (active) {
    tone(ALERT_BUZZER_PIN, BUZZER_TONE_HZ);
    return;
  }

  noTone(ALERT_BUZZER_PIN);
}
