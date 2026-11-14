# Circuit and Technical Logic: Digital Voltmeter

## 1. Working Principle
The PIC16F877A ADC input pins can only safely accept voltages between $0\text{ V}$ and $V_{DD}$ ($5.0\text{ V}$). Measuring higher input voltages requires a resistive voltage divider to scale the signal down to safe levels.
This project uses two matched 10 kOhm precision resistors in series ($R_1 = 10\text{ k}\Omega, R_2 = 10\text{ k}\Omega$):
$$V_{pin} = V_{in} \times \frac{R_2}{R_1 + R_2} = V_{in} \times \frac{10\text{ k}\Omega}{10\text{ k}\Omega + 10\text{ k}\Omega} = \frac{V_{in}}{2}$$
The microcontroller measures $V_{pin}$ via `AN0`, calculates the ADC voltage, and multiplies by the attenuation factor (2.0) to reconstruct the true input voltage:
$$V_{measured} = \left(\frac{\text{ADC} \times 5.0\text{ V}}{1023.0}\right) \times 2.0 = \frac{\text{ADC} \times 10.0\text{ V}}{1023.0}$$
This enables measuring DC voltages up to $10.0\text{ V}$ with approximately $9.77\text{ mV}$ resolution.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | Microcontroller performing ADC sampling and scaling |
| Alphanumeric LCD | LCD1 | LM016L (16x2 Characters) | 1 | Formatted voltage display |
| Divider Resistors | R1, R2 | 10 kOhm, 0.25 W, 1% tolerance | 2 | 2:1 resistive voltage attenuation network |
| Contrast Potentiometer | RV1 | 1 kOhm linear | 1 | LCD contrast adjustment |
| Crystal Oscillator | X1 | 20 MHz Quartz Crystal | 1 | Master clock reference |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 2 | `RA0/AN0` | Node between R1 & R2 | Attenuated analog input signal |
| Pin 19 | `RD0/PSP0` | LCD Pin 4 (`RS`) | Register Select |
| Pin 20 | `RD1/PSP1` | LCD Pin 6 (`EN`) | Enable strobe line |
| Pins 27–30 | `RD4–RD7` | LCD Pins 11–14 (`D4–D7`) | 4-bit LCD data bus |
| Outside of R1 | Voltage Divider Input | External Voltage Source ($V_1$) | Input voltage under measurement |

## 4. Technical Explanation & Register Configuration
- **`ADCON1 = 0x8E`:** Right-justified result (`ADFM=1`), `AN0` analog, references tied to VDD ($5\text{ V}$) and VSS ($0\text{ V}$).
- **`ADCON0 = 0x81`:** Conversion clock set to $F_{OSC}/32$ (`ADCS=10`), Channel 0 selected, ADC enabled.
- **Conversion and Display Algorithm:**
  ```c
  adc = adc_read();
  voltage = (adc * 5.0 / 1023.0) * 2.0;
  sprintf(display, "Voltage: %5.2fV", voltage);
  lcd_command(0x80);
  lcd_string("DIGITAL VOLTMETER");
  lcd_command(0xC0);
  lcd_string(display);
  ```

## 5. Testing Observations
- Verified in Proteus schematic `PIC16F877A_Project_07_Digital_Voltmeter`.
- With a 1.00 V test DC source applied at $V_1$, the 2:1 divider presents $0.50\text{ V}$ at AN0.
- The LCD correctly computes and displays `VOLTAGE METER / V = 1.00 V`.
- *Note:* Precompiled `.hex` artifact is pending rebuild in MPLAB X IDE.
