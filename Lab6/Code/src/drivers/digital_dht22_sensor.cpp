#include "digital_dht22_sensor.h"

#include <Arduino.h>
#include <DHT.h>
#include <math.h>

#include "app_config.h"
#include "signal_conversion.h"

namespace {
DHT g_dht(DHT_PIN, DHT11);

constexpr uint32_t DHT_MIN_READ_INTERVAL_MS = 2000U;

DigitalTemperatureSample g_cachedSample = {0.0F, false};
uint32_t g_lastReadMs = 0U;
bool g_hasSample = false;

bool isPlausibleDhtTemperature(float temperatureC) {
  return !isnan(temperatureC) && temperatureC >= DIGITAL_VALID_MIN_C &&
         temperatureC <= DIGITAL_VALID_MAX_C;
}

bool isConvertibleRawLikeValue(float value) {
  return !isnan(value) && value >= 1.0F && value <= static_cast<float>(ADC_MAX_VALUE - 1U);
}
}  // namespace

void digitalDht22SensorInit() {
  g_dht.begin();
  g_cachedSample = {0.0F, false};
  g_lastReadMs = 0U;
  g_hasSample = false;
}

DigitalTemperatureSample digitalDht22SensorRead() {
  const uint32_t nowMs = millis();
  if (g_hasSample && (nowMs - g_lastReadMs) < DHT_MIN_READ_INTERVAL_MS) {
    return g_cachedSample;
  }

  const float temperatureC = g_dht.readTemperature();
  g_lastReadMs = nowMs;

  if (isPlausibleDhtTemperature(temperatureC)) {
    g_cachedSample = {temperatureC, true};
    g_hasSample = true;
  } else if (isConvertibleRawLikeValue(temperatureC)) {
    const float estimatedTemperatureC = ntcAdcToCelsius(temperatureC);
    if (isPlausibleDhtTemperature(estimatedTemperatureC)) {
      g_cachedSample = {estimatedTemperatureC, true};
      g_hasSample = true;
    }
  }

  return g_cachedSample;
}
