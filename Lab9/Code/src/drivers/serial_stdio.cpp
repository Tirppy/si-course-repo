#include "drivers/serial_stdio.h"

#include <Arduino.h>

void serialStdioInit(unsigned long baudRate) { Serial.begin(baudRate); }

void serialStdioPrintHelp() {
  Serial.println(F("Lab9 Part1 ON-OFF thermistor controller"));
  Serial.println(F("Commands:"));
  Serial.println(F("  SET <tempC>  - set serial setpoint in range 18.0..45.0"));
  Serial.println(F("  UP           - increase serial setpoint by 0.5 C"));
  Serial.println(F("  DOWN         - decrease serial setpoint by 0.5 C"));
  Serial.println(F("  AUTO         - use potentiometer as setpoint source"));
  Serial.println(F("  HYST <value> - set hysteresis width in range 0.2..10.0 C"));
  Serial.println(F("  TMIN <value> - set acceptable minimum temperature"));
  Serial.println(F("  TMAX <value> - set acceptable maximum temperature"));
  Serial.println(F("  REPORT ON    - enable periodic reporting"));
  Serial.println(F("  REPORT OFF   - disable periodic reporting"));
  Serial.println(F("  REPORT NOW   - print one report immediately"));
  Serial.println(F("  HELP         - print this help"));
  Serial.println(F("Plotter fields: SetPoint Value Output"));
}

void serialStdioPrintStatus(float measuredTemperatureC,
                            float setpointTemperatureC,
                            float hysteresisWidthC,
                            float hysteresisLowC,
                            float hysteresisHighC,
                            float acceptableMinTemperatureC,
                            float acceptableMaxTemperatureC,
                            bool temperatureWithinRange,
                            bool relayOn,
                            bool serialOverrideEnabled,
                            const char* sourceLabel) {
  Serial.print(F("Mode="));
  Serial.print(serialOverrideEnabled ? F("SERIAL") : F("POT"));
  Serial.print(F(" Source="));
  Serial.print(sourceLabel);
  Serial.print(F(" SetPoint="));
  Serial.print(setpointTemperatureC, 2);
  Serial.print(F("C Value="));
  Serial.print(measuredTemperatureC, 2);
  Serial.print(F("C Hyst="));
  Serial.print(hysteresisWidthC, 2);
  Serial.print(F("C HLow="));
  Serial.print(hysteresisLowC, 2);
  Serial.print(F("C HHigh="));
  Serial.print(hysteresisHighC, 2);
  Serial.print(F("C Tmin="));
  Serial.print(acceptableMinTemperatureC, 2);
  Serial.print(F("C Tmax="));
  Serial.print(acceptableMaxTemperatureC, 2);
  Serial.print(F("C Range="));
  Serial.print(temperatureWithinRange ? F("OK") : F("ALARM"));
  Serial.print(F("C Output="));
  Serial.println(relayOn ? F("ON") : F("OFF"));

  Serial.print(F("SetPoint:"));
  Serial.print(setpointTemperatureC, 2);
  Serial.print(' ');
  Serial.print(F("Value:"));
  Serial.print(measuredTemperatureC, 2);
  Serial.print(' ');
  Serial.print(F("Output:"));
  Serial.println(relayOn ? 1 : 0);
}

void serialStdioPrintMessage(const char* message) {
  Serial.print(F("[INFO] "));
  Serial.println(message);
}
