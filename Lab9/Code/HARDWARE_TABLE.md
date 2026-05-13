# Lab9 Hardware Table

## Arduino Uno port map

| Arduino port | Connect to | Notes |
|---|---|---|
| `A0` | Thermistor divider node | Junction between thermistor and fixed 10k resistor |
| `A1` | Potentiometer `SIG` / wiper | Setpoint input |
| `D8` | Heater resistor branch and indicator LED branch | Binary ON-OFF output |
| `D7` | LCD `RS` | LCD control pin |
| `D6` | LCD `E` | LCD enable pin |
| `D5` | LCD `D4` | LCD data pin |
| `D4` | LCD `D5` | LCD data pin |
| `D3` | LCD `D6` | LCD data pin |
| `D13` | LCD `D7` | LCD data pin |
| `5V` | Thermistor top, potentiometer `VCC`, LCD `VDD`, LCD backlight via resistor | Main supply |
| `GND` | Fixed 10k resistor, potentiometer `GND`, heater branch return, LED cathode, LCD `VSS`, LCD `RW`, LCD `K`, LCD contrast resistor | Common ground |
| `D0/D1` | USB serial interface | Used for reporting and serial commands |

## Component wiring

| Component | Pin / terminal | Connect to | Notes |
|---|---|---|---|
| Thermistor `10k NTC Beta 3950` | Pin 1 | `5V` | Upper divider leg |
| Thermistor `10k NTC Beta 3950` | Pin 2 | `A0` | Divider midpoint |
| Fixed resistor `10k` | Pin 1 | `A0` | Lower divider leg |
| Fixed resistor `10k` | Pin 2 | `GND` | Lower divider leg |
| Potentiometer | `VCC` | `5V` | Setpoint supply |
| Potentiometer | `GND` | `GND` | Setpoint reference |
| Potentiometer | `SIG` / middle pin | `A1` | Setpoint signal |
| Heater resistor `220 ohm` | Pin 1 | `D8` | Heating output proxy |
| Heater resistor `220 ohm` | Pin 2 | `GND` | Heating output return |
| Indicator LED resistor `220 ohm` | Pin 1 | `D8` | LED current limiting resistor |
| Indicator LED resistor `220 ohm` | Pin 2 | LED anode | LED branch |
| Indicator LED | Anode | LED resistor output | Active-high indicator |
| Indicator LED | Cathode | `GND` | Return path |
| LCD 1602 | `VSS` | `GND` | Logic ground |
| LCD 1602 | `VDD` | `5V` | Logic supply |
| LCD 1602 | `VO` | `10k` resistor to `GND` | Contrast path used in this lab |
| LCD 1602 | `RS` | `D7` | Control pin |
| LCD 1602 | `RW` | `GND` | Write mode only |
| LCD 1602 | `E` | `D6` | Enable pin |
| LCD 1602 | `D4` | `D5` | Data pin |
| LCD 1602 | `D5` | `D4` | Data pin |
| LCD 1602 | `D6` | `D3` | Data pin |
| LCD 1602 | `D7` | `D13` | Data pin |
| LCD 1602 | `A` | `220 ohm` resistor to `5V` | Backlight anode |
| LCD 1602 | `K` | `GND` | Backlight cathode |
| LCD contrast resistor `10k` | Pin 1 | `VO` | Contrast connection |
| LCD contrast resistor `10k` | Pin 2 | `GND` | Contrast pull-down |
| LCD backlight resistor `220 ohm` | Pin 1 | `5V` | Backlight supply |
| LCD backlight resistor `220 ohm` | Pin 2 | LCD `A` | Backlight current limiting |

## Notes

- Only the ports listed above are used by the current Lab 9 firmware.
- `D8` is active-high: when the output is `ON`, the pin goes `HIGH`.
- `D0/D1` should be left for USB serial and not reused for other hardware in this lab.
