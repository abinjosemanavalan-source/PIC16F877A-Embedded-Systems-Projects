# Circuit and Technical Logic: ADC Potentiometer

## 1. Working Principle
The PIC16F877A features a 10-bit Successive Approximation Register (SAR) Analog-to-Digital Converter (ADC). It converts an analog input voltage into a discrete 10-bit digital binary number:
$$\text{Digital ADC Value} = \text{round}\left(\frac{V_{in} - V_{ref-}}{V_{ref+} - V_{ref-}} \times (2^{10} - 1)\right) = \text{round}\left(\frac{V_{in}}{5.0\text{ V}} \times 1023\right)$$
- An input of $0.0\text{ V}$ corresponds to digital code `0`.
- An input of $2.5\text{ V}$ corresponds to digital code `511` / `512`.
- An input of $5.0\text{ V}$ corresponds to digital code `1023`.
A potentiometer connected as an adjustable voltage divider between VDD and GND provides a smooth continuous voltage into analog channel `AN0`.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | Microcontroller running ADC & LCD routines |
| Alphanumeric LCD | LCD1 | LM016L (16x2 Character Display) | 1 | Real-time ADC count display |
| Potentiometer | RV1 | 1 kOhm linear | 1 | Analog voltage generator on AN0 |
| Bias Resistor | R1 | 10 kOhm, 0.25 W | 1 | LCD contrast bias resistor |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 2 | `RA0/AN0` | Wiper of RV1 (Potentiometer) | Analog Input Channel 0 |
| Pin 19 | `RD0/PSP0` | LCD Pin 4 (`RS`) | LCD Register Select |
| Pin 20 | `RD1/PSP1` | LCD Pin 6 (`EN`) | LCD Enable strobe |
| Pins 27–30 | `RD4–RD7` | LCD Pins 11–14 (`D4–D7`) | 4-bit LCD data bus |

## 4. Technical Explanation & Register Configuration
- **`ADCON1 = 0x0E`:**
  - Bit 7 (`ADFM = 1`): Right-justified 10-bit result in `ADRESH:ADRESL`.
  - Bits 3–0 (`PCFG = 1110`): Sets `RA0/AN0` as an analog input; all other PORTA/PORTE pins operate as digital I/O. Voltage reference is set to internal rails ($V_{ref+} = V_{DD} = 5\text{ V}$, $V_{ref-} = V_{SS} = 0\text{ V}$).
- **`ADCON0 = 0x01`:**
  - Bits 7–6 (`ADCS1:ADCS0 = 00`): ADC conversion clock $F_{OSC}/2$.
  - Bits 5–3 (`CHS = 000`): Selects Channel 0 (`AN0`).
  - Bit 0 (`ADON = 1`): Powers on the ADC converter module.
- **Conversion Routine:**
  ```c
  unsigned int ADC_Read(void) {
      ADCON0bits.GO = 1;           // Initiate conversion
      while (ADCON0bits.GO);       // Hardware clears GO when conversion ends
      return ((ADRESH << 8) | ADRESL); // Return 10-bit result
  }
  ```

## 5. Testing Observations
- Verified in Proteus schematic `Project_6_ADC`.
- Sweeping potentiometer RV1 from 0% to 100% smoothly updates the LCD display across the full 10-bit range from `0` to `1023`.
