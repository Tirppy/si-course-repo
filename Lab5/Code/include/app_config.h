#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <Arduino.h>

constexpr uint8_t ANALOG_NTC_PIN = A0;
constexpr uint8_t THRESHOLD_POT_PIN = A1;
constexpr bool THRESHOLD_POT_CONNECTED = true;
constexpr uint8_t DHT11_PIN = 8;
constexpr uint8_t ANALOG_ALERT_LED_PIN = 12;
constexpr uint8_t DIGITAL_ALERT_LED_PIN = 13;
constexpr uint8_t ALERT_BUZZER_PIN = 11;

constexpr uint8_t LCD_RS_PIN = 2;
constexpr uint8_t LCD_ENABLE_PIN = 3;
constexpr uint8_t LCD_D4_PIN = 4;
constexpr uint8_t LCD_D5_PIN = 5;
constexpr uint8_t LCD_D6_PIN = 6;
constexpr uint8_t LCD_D7_PIN = 7;

constexpr uint32_t ACQUISITION_PERIOD_MS = 25;
constexpr uint32_t DISPLAY_PERIOD_MS = 500;
constexpr uint32_t REPORT_PERIOD_MS = 500;

constexpr float ALERT_THRESHOLD_MIN_C = 20.0F;
constexpr float ALERT_THRESHOLD_MAX_C = 35.0F;
constexpr float ALERT_THRESHOLD_DEFAULT_C = 27.5F;
constexpr float ALERT_HYSTERESIS_C = 1.0F;
constexpr uint8_t ALERT_CONFIRM_SAMPLES = 3;
constexpr float DIFFERENCE_ALERT_THRESHOLD_C = 6.0F;

constexpr float ANALOG_VALID_MIN_C = -40.0F;
constexpr float ANALOG_VALID_MAX_C = 125.0F;
constexpr float DIGITAL_VALID_MIN_C = -55.0F;
constexpr float DIGITAL_VALID_MAX_C = 125.0F;

constexpr uint16_t ADC_MAX_VALUE = 1023U;
constexpr float NTC_BETA = 3950.0F;
constexpr float NTC_ROOM_TEMP_K = 298.15F;

constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS = 2;

constexpr uint32_t LATENCY_BUDGET_MS = ACQUISITION_PERIOD_MS * ALERT_CONFIRM_SAMPLES;

#endif
