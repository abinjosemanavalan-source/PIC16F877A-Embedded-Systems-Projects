# Circuit and Technical Logic: Timer0 LED Scheduling

## 1. Working Principle
Delay functions (`__delay_ms()`) halt CPU instruction execution in empty software loops, preventing the microcontroller from reacting to inputs, sensors, or communications.
Timer0 scheduling replaces software delays with hardware timing:
- An internal hardware 8-bit timer (Timer0) increments on every clock cycle ($F_{OSC}/4$) scaled by a prescaler.
- When Timer0 overflows from `255` to `0`, it asserts an interrupt flag (`TMR0IF`).
- The ISR reloads the initial offset, tracks overflow counts, and sets an event flag (`blink_event = 1`).
- The main `while(1)` loop can now execute non-blocking cooperative tasks (such as immediate button debouncing and polling) without freezing.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | CPU running hardware timer scheduling |
| Quartz Crystal | X1, X2 | 20 MHz Crystal | 1 | Primary oscillator clock |
| Pushbutton | SW1 | Tactile Switch SPST-NO | 1 | Foreground asynchronous input button |
| Pull-up Resistor | R2 | 10 kOhm, 0.25 W | 1 | Pull-up on RB1 input |
| Indicator LED | D1 | Red 5 mm LED | 1 | Scheduled blinking indicator |
| Resistor | R1 | 330 Ohm, 0.25 W | 1 | Current limiting for LED D1 |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 34 | `RB1` | Pushbutton & R2 (10 kOhm pull-up) | Foreground digital input button |
| Pin 19 | `RD0/PSP0` | R1 (330 Ohm) -> Anode of D1 (Red LED) | Scheduled blinking output |
| Pins 13, 14 | `OSC1, OSC2` | 20 MHz Crystal | Main oscillator inputs |

## 4. Technical Explanation & Timing Math
- **System Clock:** $F_{OSC} = 20\text{ MHz} \implies F_{CY} = F_{OSC}/4 = 5\text{ MHz}$ ($T_{CY} = 200\text{ ns}$).
- **`OPTION_REG = 0b00000111`:**
  - `T0CS = 0`: Internal instruction cycle clock.
  - `PSA = 0`: Prescaler assigned to Timer0.
  - `PS2:PS0 = 111`: Prescaler ratio 1:256.
  - Timer tick interval = $200\text{ ns} \times 256 = 51.2\ \mu\text{s}$.
- **Overflow Interval:**
  Timer reloads `TMR0 = 100`, requiring $256 - 100 = 156$ counts to overflow:
  $$T_{overflow} = 156 \times 51.2\ \mu\text{s} \approx 7.987\text{ ms} \approx 8\text{ ms}$$
- **Event Period:**
  After 10 overflows (`timer_count >= 10`), $T_{event} = 10 \times 7.987\text{ ms} \approx 79.87\text{ ms} \approx 80\text{ ms}$.
- **Concurrent Polling:**
  The `while(1)` loop checks `if (blink_event)` to toggle the LED, while simultaneously checking `if (PORTBbits.RB1 == 0)` to respond instantly to button presses without delay freezes.

## 5. Testing Observations
- Verified in Proteus schematic `Project_4_Blink_without_freezing_program`.
- The LED toggles regularly at ~80 ms intervals.
- The button on RB1 triggers an immediate response without lag or stutter, proving true concurrent scheduling.
