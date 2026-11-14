# Circuit and Technical Logic: External Interrupt on RB0/INT

## 1. Working Principle
In polled input systems, the CPU must repeatedly inspect input pins. If the CPU is executing intensive calculations, delays, or communication routines, input events can be dropped.
The hardware external interrupt on `RB0/INT` solves this by introducing asynchronous event handling:
- When a falling edge occurs on `RB0/INT`, the hardware immediately suspends regular execution.
- The Program Counter is saved to the hardware stack, and the CPU jumps to interrupt vector `0x0004`.
- The Interrupt Service Routine (ISR) verifies `INTCONbits.INTF`, sets an application event flag (`button_event = 1`), and clears the hardware flag.
- The CPU returns to the main loop via `retfie`, where the main loop processes the recorded event at its earliest opportunity.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | Microcontroller handling interrupt and foreground tasks |
| Pushbutton | SW1 | Tactile Pushbutton SPST-NO | 1 | Asynchronous trigger switch |
| Pull-up Resistor | R2 | 10 kOhm, 0.25 W | 1 | Pull-up resistor for RB0/INT |
| Indicator LED | D1 | Red 5 mm LED | 1 | Visual event indicator |
| Current Limiting Resistor | R1 | 330 Ohm, 0.25 W | 1 | Current protection for LED D1 |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 33 | `RB0/INT` | Pushbutton & R2 (10 kOhm pull-up) | Dedicated hardware external interrupt pin |
| Pin 19 | `RD0/PSP0` | R1 (330 Ohm) -> Anode of D1 (Red LED) | Output indicator LED |
| Pins 11, 32 | `VDD` | +5.0 V | Power supply |
| Pins 12, 31 | `VSS` | GND (0 V) | Ground |

## 4. Technical Explanation & Register Configuration
- **`OPTION_REGbits.INTEDG = 0`:** Configures interrupt trigger on the falling edge (high-to-low transition when active-low button is pressed).
- **`INTCONbits.INTE = 1`:** Enables the `RB0/INT` external interrupt.
- **`INTCONbits.GIE = 1`:** Enables global interrupts.
- **ISR Architecture:**
  ```c
  void __interrupt() ISR(void) {
      if (INTCONbits.INTF == 1) {
          button_event = 1;       // Record event asynchronously
          INTCONbits.INTF = 0;    // Clear flag to enable future interrupts
      }
  }
  ```
- **Foreground Main Loop:**
  Main loop continuously blinks LED RD0 with 300 ms delays. If `button_event == 1`, it immediately recognizes the flag, forces RD0 ON for 1000 ms, resets the flag, and returns to normal blinking.

## 5. Testing Observations
- Verified in Proteus schematic `Project_3 External_Interrupt`.
- The button press is captured instantly regardless of whether the processor is currently in the middle of a 300 ms delay. Zero button events are lost.
