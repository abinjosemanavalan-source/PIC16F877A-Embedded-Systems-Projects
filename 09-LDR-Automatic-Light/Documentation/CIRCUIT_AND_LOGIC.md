# Circuit and Technical Logic: LDR Automatic Light

## 1. Working Principle
A Light Dependent Resistor (LDR / photoresistor) exhibits electrical resistance that decreases non-linearly with increasing ambient illumination:
- In total darkness, LDR resistance is very high (often $> 1\text{ M}\Omega$).
- In bright light, LDR resistance drops to a few hundred ohms.
By placing the LDR in a voltage divider with fixed resistor $R_2$ ($10\text{ k}\Omega$):
$$V_{out} = V_{DD} \times \frac{R_2}{R_{LDR} + R_2}$$
As light levels increase, $R_{LDR}$ decreases, causing $V_{out}$ to rise toward $5\text{ V}$. In darkness, $R_{LDR}$ rises, pulling $V_{out}$ toward $0\text{ V}$.

To prevent rapid oscillation (relay chattering / LED flickering) near the transition boundary, the firmware implements **dual-threshold Schmitt trigger hysteresis**:
- **Turn ON Threshold:** If ADC reading drops below `400` (Dark condition), the light is switched ON (`RC0 = 1`).
- **Turn OFF Threshold:** If ADC reading rises above `600` (Bright condition), the light is switched OFF (`RC0 = 0`).
- **Hysteresis Band (400 to 600):** If the light reading lies between 400 and 600, the output retains its previous state, preventing flicker caused by sensor noise.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | Microcontroller executing ADC and hysteresis control |
| Photoresistor | LDR1 | Light Dependent Resistor | 1 | Ambient light sensor |
| Fixed Resistor | R2 | 10 kOhm, 0.25 W | 1 | Voltage divider load resistor |
| Output LED | D1 | Red 5 mm LED | 1 | Automatic light emitter |
| Current Limiting Resistor | R1 | 330 Ohm, 0.25 W | 1 | Current protection for LED D1 |
| Alphanumeric LCD | LCD1 | LM016L (16x2 Characters) | 1 | Real-time LDR value and status display |
| Crystal Oscillator | X1 | 20 MHz Quartz Crystal | 1 | Master clock reference |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 2 | `RA0/AN0` | Node between LDR1 and R2 | Analog light level input |
| Pin 15 | `RC0/T1OSO/T1CKI` | R1 (330 Ohm) -> Anode of D1 (Red LED) | Automatic light drive output |
| Pin 33 | `RB0/INT` | LCD Pin 4 (`RS`) | LCD Register Select |
| Pin 34 | `RB1` | LCD Pin 6 (`EN`) | LCD Enable strobe line |
| Pins 37–40 | `RB4–RB7` | LCD Pins 11–14 (`D4–D7`) | 4-bit LCD data bus |

## 4. Technical Explanation & Register Configuration
- **`ADCON1 = 0x8E`:** Right-justified, AN0 analog, references to VDD and VSS.
- **`ADCON0 = 0x41`:** Conversion clock $F_{OSC}/8$, Channel 0 enabled, ADC ON.
- **`TRISA = 0x01, TRISB = 0x00, TRISC = 0x00`:** Sets RA0 as input, PORTB and PORTC as outputs.
- **Hysteresis Algorithm:**
  ```c
  ldr = ADC_Read();
  if (ldr < 400) {
      RC0 = 1;                           // Turn Light ON
      LCD_String("DARK: LIGHT ON ");
  } else if (ldr > 600) {
      RC0 = 0;                           // Turn Light OFF
      LCD_String("BRIGHT: LIGHT OFF");
  }
  ```

## 5. Testing Observations
- Verified in Proteus schematic `Project_09_Automatic_Light`.
- The LCD displays real-time ambient lux readings: `LDR VALUE: 540`.
- Decreasing light past 400 turns on the light; increasing light past 600 turns off the light. No fluttering occurs between 400 and 600.
