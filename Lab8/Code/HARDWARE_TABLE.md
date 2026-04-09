| Part | Model / type | Purpose | Arduino pins / ports | Extra interface notes |
| --- | --- | --- | --- | --- |
| Microcontroller | Arduino Uno | Main controller | `D0-D13`, `A0-A5`, `5V`, `GND`, `VIN`, `USB` | Central board for all connections |
| Binary actuator LED | 5mm LED + `220` to `330` ohm resistor | Binary output indicator | `D12` -> resistor -> LED anode, LED cathode -> `GND` | Used as the binary actuator output in the current implementation |
| Analog actuator output | PWM proxy LED or DC motor driver input | Analog actuator control path | `D10` -> PWM input / resistor+LED | In simulation, represented by LED brightness; in real hardware, intended for a DC motor driver PWM input |
| Motor direction control | DC motor driver direction input | Selects motor direction | `D11` -> direction input | Used by software to reverse direction when the binary actuator state is active |
| LCD | 16x2 LCD, 16-pin parallel mode | Local status display | `D7` -> `RS`, `D6` -> `E`, `D5` -> `D4`, `D4` -> `D5`, `D3` -> `D6`, `D13` -> `D7` | `VSS`->`GND`, `VDD`->`5V`, `RW`->`GND`, `VO`->`GND` in simplified Wokwi setup, backlight to `5V/GND` |
| Pushbutton | Momentary pushbutton | Binary hardware input | `D2` -> signal | Uses `INPUT_PULLUP`, other side to `GND` |
| Potentiometer | 10k potentiometer | Analog hardware input | `A0` -> wiper | Side pins to `5V` and `GND` |
| Serial interface | USB serial | Command and telemetry link | `D0/D1` internally via USB | Used by Serial Monitor at `115200` baud |

Notes:
- The current Lab 7 codebase uses `D12` for the binary actuator indicator and `D10` for the analog actuator PWM path.
- `D11` is reserved for motor direction control in the DC-motor version of the application.
- The Wokwi diagram is intentionally simplified: LCD contrast is fixed and motor direction is not shown with a separate LED.
