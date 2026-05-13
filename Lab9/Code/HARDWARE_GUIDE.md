# Lab9 Hardware Guide

This implementation uses Part 1 only: ON-OFF control with hysteresis, without FreeRTOS.

## Real hardware wiring

### Thermistor divider
- `A0` -> divider midpoint
- `5V` -> thermistor -> midpoint
- midpoint -> `10k` resistor -> `GND`

This means the thermistor is the upper leg and the fixed resistor is the lower leg.

### Setpoint input
- `A1` -> potentiometer wiper
- potentiometer side pins -> `5V` and `GND`

### Output proxy
- `D8` -> heater resistor `220R` -> `GND`
- `D8` -> LED anode through `220R` resistor
- LED cathode -> `GND`

This acts as the ON/OFF output path and indicator.

Important: this wiring is active-high in software. When output is ON, `D8` goes HIGH and the heater resistor plus indicator LED are powered.

If the LED never lights, reverse the LED orientation. The anode must go to `D8` through the resistor, and the cathode must go to `GND`.

### LCD 16x2 in 4-bit mode
- `D7` -> `RS`
- `D6` -> `E`
- `D5` -> `D4`
- `D4` -> `D5`
- `D3` -> `D6`
- `D13` -> `D7`
- `5V` -> `VCC`
- `GND` -> `GND`
- `GND` -> `RW`
- `VO` -> `10k` resistor -> `GND`
- `A` -> `220R` resistor -> `5V`
- `K` -> `GND`

Without the `VO` contrast connection, the LCD may look blank even if the software works.

## Software behavior
- `A0` reads the thermistor divider.
- `A1` sets the temperature setpoint when AUTO mode is active.
- `D8` drives the heater resistor and LED output indicator with ON-OFF control and hysteresis.
- LCD and serial show `SetPoint`, measured `Value`, and relay `Output`.
- Serial commands supported: `SET <value>`, `UP`, `DOWN`, `AUTO`, `HYST <value>`, `TMIN <value>`, `TMAX <value>`, `REPORT ON`, `REPORT OFF`, `REPORT NOW`, `HELP`.

## Wokwi simulation note
Wokwi does not provide a realistic analog thermistor-divider component chain for this exact setup as simply as real hardware, so the simulation uses a potentiometer on `A0` to emulate the divider feedback signal while preserving the same software path and thermistor conversion logic.

## Thermistor check
If the thermistor value is invalid on hardware, verify this exact divider:
- `5V` -> thermistor -> `A0`
- `A0` -> `10k` resistor -> `GND`

The code assumes the thermistor is the upper leg and the fixed resistor is the lower leg.

Do not place the heater resistor too close to the thermistor during basic testing, otherwise the measured temperature will rise quickly above room temperature even when the control output is active.
