| Part | Model / type | Purpose | Arduino pins / ports | Extra interface notes |
| --- | --- | --- | --- | --- |
| Microcontroller | Arduino Uno | Main controller | `D0-D13`, `A0-A5`, `5V`, `GND`, `VIN`, `USB` | Central board for all connections |
| LED indicator | 5mm LED + `220` to `330` ohm resistor | Binary output indicator | `D12` -> resistor -> LED anode, LED cathode -> `GND` | External LED substitute for binary actuator signaling |
| Stepper motor | `28BYJ-48` with `ULN2003` driver board | Variable actuator output | `D8` -> `IN1`, `D9` -> `IN2`, `D10` -> `IN3`, `D11` -> `IN4` | Driver board powered from `5V` and `GND` |
| LCD | 16x2 LCD, 16-pin parallel mode | Local status display | `D7` -> `RS`, `D6` -> `E`, `D5` -> `D4`, `D4` -> `D5`, `D3` -> `D6`, `D13` -> `D7` | `VSS`->`GND`, `VDD`->`5V`, `RW`->`GND`, `VO` via contrast trimmer, backlight via resistor |
| Pushbutton | Momentary pushbutton | Binary hardware input | `D2` -> signal | Uses `INPUT_PULLUP`, other side to `GND` |
| Potentiometer | 10k potentiometer | Analog hardware input | `A0` -> wiper | Side pins to `5V` and `GND` |
| Serial interface | USB serial | Command and telemetry link | `D0/D1` internally via USB | Used by Serial Monitor at `115200` baud |
| LCD contrast trimmer | 10k potentiometer | LCD contrast control | not read by MCU | Wiper to LCD `VO` |

Notes:
- The current Lab 7 codebase uses `D12` for an external binary LED indicator.
- The pushbutton on `D2` toggles the LED indicator state in hardware-enabled modes.
