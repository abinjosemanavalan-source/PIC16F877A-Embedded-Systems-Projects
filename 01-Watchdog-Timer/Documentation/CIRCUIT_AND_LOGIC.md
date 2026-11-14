# Circuit and Technical Logic: Watchdog Timer

## 1. Working Principle
The Watchdog Timer (WDT) is a dedicated on-chip free-running RC oscillator that operates completely independent of the external high-frequency crystal oscillator (OSC1/OSC2). It serves as an autonomous hardware safety supervisor:
- Under normal operating conditions, firmware cyclically executes the `CLRWDT` instruction (XC8 macro `CLRWDT()`), clearing the timer back to zero.
- If firmware enters an unhandled lockup, infinite loop, or crashes due to electrical noise, `CLRWDT()` is never executed.
- When the internal counter overflows, the WDT triggers an internal device reset, forcing the Program Counter back to address `0x0000` (Reset Vector) and restarting normal system operation.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P (40-pin DIP) | 1 | Microcontroller running firmware |
| Current Limiting Resistors | R1, R2, R3 | 220 Ohm, 0.25 W | 3 | Current protection for LEDs |
| Indicator LEDs | D1, D2, D3 | 5 mm Red LEDs | 3 | Normal stage and failure stage visual indicators |
| Crystal Oscillator | X1 | 20 MHz Quartz Crystal | 1 | System clock source |
| Power Rail | VDD, VSS | +5.0 V DC / Ground | 1 | Logic power |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 19 | `RD0/PSP0` | R1 (220 Ohm) -> Anode of D1 (Red LED) | Normal operation blink indicator |
| Pin 20 | `RD1/PSP1` | R2 (220 Ohm) -> Anode of D2 (Red LED) | Fault state indicator (turns on before stall) |
| Pin 21 | `RD2/PSP2` | R3 (220 Ohm) -> Anode of D3 (Red LED) | Auxiliary pin on schematic (logic LOW) |
| Pin 1 | `MCLR/Vpp/THV` | +5.0 V | Master Clear Reset |
| Pins 11, 32 | `VDD` | +5.0 V | Power supply input |
| Pins 12, 31 | `VSS` | GND (0 V) | Ground reference |

## 4. Technical Explanation & Register Configuration
- **Configuration Bits:** `#pragma config WDTE = ON` activates the Watchdog Timer at hardware reset.
- **`OPTION_REG` Configuration:**
  - `PSA = 1`: Assigns the internal prescaler directly to the WDT (disassociating it from Timer0).
  - `PS2 = 1, PS1 = 1, PS0 = 1`: Prescaler ratio set to `1:128`.
  - Nominal baseline WDT timeout period = 18 ms.
  - Effective timeout period $T_{WDT} \approx 18\text{ ms} \times 128 \approx 2.304\text{ seconds}$.
- **Execution Flow:**
  - Stage 1: `RD0` blinks 7 times (300 ms ON / 300 ms OFF). In every loop cycle, `CLRWDT()` is called to prevent timeout.
  - Stage 2: `RD0` turns OFF, `RD1` turns ON.
  - Stage 3: Program enters an intentional `while(1) {}` trap without servicing the watchdog.
  - Stage 4: After ~2.3 seconds, WDT overflows, resetting the microcontroller and restarting Stage 1.

## 5. Testing Observations
- Verified in Proteus schematic `project_01_watchdog_Timer`.
- LED D1 blinks cyclically during healthy execution.
- LED D2 illuminates steadily when entering the stall phase.
- The PIC resets automatically after ~2.3 seconds without human intervention, confirming autonomous crash recovery.
