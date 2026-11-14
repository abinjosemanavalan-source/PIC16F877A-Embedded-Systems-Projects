# Circuit and Technical Logic: Floating Input Demonstration

## 1. Working Principle
Microcontroller CMOS digital input pins have extremely high input impedance (typically $> 10^{12}\ \Omega$). When an input pin is left unconnected ("floating"), it acts as an antenna:
- Ambient electromagnetic radiation, electrostatic charges, and parasitic capacitive coupling from neighboring PCB traces can easily induce erratic voltages.
- The pin voltage drifts unpredictably between $0\text{ V}$ and $5\text{ V}$, frequently hovering near the CMOS logic threshold ($V_{IH}/V_{IL}$ switching point).
- This causes random output toggling, excessive internal shoot-through current in input buffers, and unpredictable software behavior.
Adding an external pull-up or pull-down resistor forces the pin to a known, stable reference voltage when the switch is open.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | Microcontroller running input test firmware |
| Indicator LED | D1 | Blue 5 mm LED | 1 | Visual logic level output indicator |
| Current Limiting Resistor | R1 | 330 Ohm, 0.25 W | 1 | LED current limiting |
| Pull-up / Biasing Resistor | R2 | 330 Ohm, 0.25 W | 1 | Input bias resistor (connected to VDD) |
| Tactile Switch | SW1 | SPST Pushbutton | 1 | Digital input trigger |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 15 | `RC0/T1OSO/T1CKI` | Switch & R2 (330 Ohm pull-up) | Digital input pin under test |
| Pin 19 | `RD0/PSP0` | R1 (330 Ohm) -> Anode of D1 (Blue LED) | Output status indicator |
| Pins 11, 32 | `VDD` | +5.0 V | Logic supply |
| Pins 12, 31 | `VSS` | GND (0 V) | Ground |

## 4. Technical Explanation & Register Configuration
- **Register Directions:**
  - `TRISCbits.TRISC0 = 1`: Pin `RC0` configured as a high-impedance digital input.
  - `TRISDbits.TRISD0 = 0`: Pin `RD0` configured as a push-pull digital output.
- **Firmware Logic:**
  ```c
  if (PORTCbits.RC0 == 0) {
      PORTDbits.RD0 = 1;  // Button pressed: LED ON
  } else {
      PORTDbits.RD0 = 0;  // Button released: LED OFF
  }
  ```
- **Observations on Biasing:**
  - Without R2 (floating pin): In physical environments or simulations with noise sources, touching the pin or leaving it open causes RD0 to flicker erratically.
  - With R2 connected to +5V: Pin RC0 is tied securely to logic HIGH ($5.0\text{ V}$) when open, and pulled solidly to GND ($0.0\text{ V}$) when pressed, guaranteeing deterministic behavior.

## 5. Testing Observations
- Verified in Proteus schematic `Project_5_Floating_Input`.
- With the pull-up resistor circuit configured, pressing the button switches the blue LED cleanly without spurious triggering.
