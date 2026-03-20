#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <Arduino.h>

constexpr uint8_t ANALOG_SENSOR_PIN = A0;
constexpr uint8_t PERIOD_CONTROL_PIN = A1;
constexpr uint8_t DHT_PIN = 8;
constexpr uint8_t NOISE_BUTTON_PIN = 9;
constexpr uint8_t TREND_LED_PIN = 10;
constexpr uint8_t ANALOG_ALERT_LED_PIN = 12;
constexpr uint8_t DIGITAL_ALERT_LED_PIN = 13;

constexpr uint8_t LCD_RS_PIN = 2;
constexpr uint8_t LCD_ENABLE_PIN = 3;
constexpr uint8_t LCD_D4_PIN = 4;
constexpr uint8_t LCD_D5_PIN = 5;
constexpr uint8_t LCD_D6_PIN = 6;
constexpr uint8_t LCD_D7_PIN = 7;

constexpr uint32_t ACQUISITION_PERIOD_DEFAULT_MS = 50U;
constexpr uint32_t ACQUISITION_PERIOD_MIN_MS = 20U;
constexpr uint32_t ACQUISITION_PERIOD_MAX_MS = 100U;
constexpr uint32_t DISPLAY_PERIOD_MS = 500U;
constexpr uint32_t REPORT_PERIOD_DEFAULT_MS = 1000U;
constexpr uint32_t REPORT_PERIOD_FACTOR = 15U;
constexpr uint32_t REPORT_PERIOD_MIN_MS = 500U;
constexpr uint32_t REPORT_PERIOD_MAX_MS = 1500U;

constexpr uint16_t ANALOG_SATURATION_MIN = 200U;
constexpr uint16_t ANALOG_SATURATION_MAX = 850U;

constexpr float DIGITAL_SATURATION_MIN_C = -20.0F;
constexpr float DIGITAL_SATURATION_MAX_C = 60.0F;

constexpr uint8_t MEDIAN_WINDOW_SIZE = 5U;
constexpr uint8_t WEIGHTED_WINDOW_SIZE = 4U;
constexpr float WEIGHTED_FILTER_WEIGHTS[WEIGHTED_WINDOW_SIZE] = {35.0F, 30.0F, 20.0F, 15.0F};

constexpr float ALERT_THRESHOLD_C = 30.0F;
constexpr float ALERT_HYSTERESIS_C = 1.0F;
constexpr uint8_t ALERT_CONFIRM_SAMPLES = 2U;

constexpr float ANALOG_VALID_MIN_C = -40.0F;
constexpr float ANALOG_VALID_MAX_C = 125.0F;
constexpr float DIGITAL_VALID_MIN_C = -40.0F;
constexpr float DIGITAL_VALID_MAX_C = 85.0F;

constexpr uint16_t ADC_MAX_VALUE = 1023U;
constexpr float NTC_BETA = 3950.0F;
constexpr float NTC_ROOM_TEMP_K = 298.15F;

constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS = 2;

constexpr float TREND_DELTA_THRESHOLD_C = 0.15F;
constexpr uint8_t TREND_STABLE_SAMPLE_COUNT = 40U;

constexpr float ANALOG_FAST_CHANGE_THRESHOLD_C = 1.5F;
constexpr float DIGITAL_FAST_CHANGE_THRESHOLD_C = 1.0F;
constexpr uint32_t FAST_CHANGE_LED_HOLD_MS = 2000U;

constexpr int16_t ANALOG_WHITE_NOISE_AMPLITUDE = 15;
constexpr int16_t ANALOG_SPIKE_NOISE_AMPLITUDE = 60;
constexpr uint8_t ANALOG_SPIKE_PROBABILITY_PERCENT = 5U;
constexpr float DIGITAL_WHITE_NOISE_AMPLITUDE_C = 1.8F;

#endif
