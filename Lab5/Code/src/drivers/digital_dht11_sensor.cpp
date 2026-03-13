#include "digital_dht11_sensor.h"

#include <Arduino.h>
#include <DHT.h>
#include <math.h>

#include "app_config.h"
#include "signal_conversion.h"

namespace {
DHT g_dht11(DHT11_PIN, DHT11);
DHT g_dht22(DHT11_PIN, DHT22);

constexpr uint32_t DHT_MIN_READ_INTERVAL_MS = 2000U;

DigitalTemperatureSample g_cachedSample = {
    NAN,
    NAN,
    false,
};

uint32_t g_lastReadMs = 0U;
bool g_hasSample = false;

bool isHumidityValid(float humidityPercent) {
  return !isnan(humidityPercent) && humidityPercent >= 0.0F && humidityPercent <= 100.0F;
}

DigitalTemperatureSample makeSample(float temperatureC, float humidityPercent) {
  const bool temperatureValid = isDigitalTemperatureValid(temperatureC);
  const bool humidityValid = isHumidityValid(humidityPercent);

  return {
      temperatureValid ? temperatureC : NAN,
      humidityValid ? humidityPercent : NAN,
      temperatureValid,
  };
}
}

void digitalDht11SensorInit() {
  g_dht11.begin();
  g_dht22.begin();
  g_cachedSample = {NAN, NAN, false};
  g_lastReadMs = 0U;
  g_hasSample = false;
}

DigitalTemperatureSample digitalDht11SensorRead() {
  const uint32_t nowMs = millis();
  if (g_hasSample && (nowMs - g_lastReadMs) < DHT_MIN_READ_INTERVAL_MS) {
    return g_cachedSample;
  }

  const DigitalTemperatureSample dht22Sample =
      makeSample(g_dht22.readTemperature(), g_dht22.readHumidity());
  const DigitalTemperatureSample dht11Sample =
      makeSample(g_dht11.readTemperature(), g_dht11.readHumidity());

  g_cachedSample = dht22Sample.valid ? dht22Sample : dht11Sample;

  g_lastReadMs = nowMs;
  g_hasSample = true;
  return g_cachedSample;
}
