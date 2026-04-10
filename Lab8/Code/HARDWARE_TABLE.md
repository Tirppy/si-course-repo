| Part | Model / type | Purpose | Arduino pins / ports | Extra interface notes |
| --- | --- | --- | --- | --- |
| Microcontroller | Arduino Uno | Main controller | `D0-D13`, `A0-A5`, `5V`, `GND`, `VIN`, `USB` | Central board for all connections |
| Binary actuator LED | 5mm LED + `220` to `330` ohm resistor | Binary output indicator | `D12` -> resistor -> LED anode, LED cathode -> `GND` | Used as the binary actuator output in the current implementation |
| L293D enable input | `L293D EN1,2` | PWM speed control for DC motor | `D10` -> `EN1,2` | PWM speed control for the motor driver |
| L293D direction input 1 | `L293D IN1` | DC motor direction control | `D8` -> `IN1` | Used for forward direction command |
| L293D direction input 2 | `L293D IN2` | DC motor direction control | `D9` -> `IN2` | Used for reverse direction command |
| Analog actuator output | DC motor through `L293D` or PWM proxy LED | Analog actuator control path | `OUT1/OUT2` from `L293D` to motor | In simulation, represented by LED brightness on `OUT1`; in real hardware, connect a DC motor to `OUT1/OUT2` |
| LCD | 16x2 LCD, 16-pin parallel mode | Local status display | `D7` -> `RS`, `D6` -> `E`, `D5` -> `D4`, `D4` -> `D5`, `D3` -> `D6`, `D13` -> `D7` | `VSS`->`GND`, `VDD`->`5V`, `RW`->`GND`, `VO`->`GND` in simplified Wokwi setup, backlight to `5V/GND` |
| Pushbutton | Momentary pushbutton | Binary hardware input | `D2` -> signal | Uses `INPUT_PULLUP`, other side to `GND` |
| Potentiometer | 10k potentiometer | Analog hardware input | `A0` -> wiper | Side pins to `5V` and `GND` |
| Serial interface | USB serial | Command and telemetry link | `D0/D1` internally via USB | Used by Serial Monitor at `115200` baud |

Notes:
- The current Lab 7 codebase uses `D10`, `D8`, and `D9` for proper `L293D` motor control (`EN1,2`, `IN1`, `IN2`).
- When speed is `0%`, the software disables both direction lines so the motor stops.
- The Wokwi diagram is intentionally simplified: LCD contrast is fixed and motor direction is not shown with a separate LED.
