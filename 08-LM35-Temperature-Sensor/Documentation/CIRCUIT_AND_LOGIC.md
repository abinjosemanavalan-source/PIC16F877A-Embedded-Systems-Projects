# Circuit and Technical Logic: LM35 Temperature Sensor

## 1. Working Principle
The LM35 is an integrated-circuit precision centigrade temperature sensor whose output voltage is linearly proportional to the Celsius temperature:
$$V_{out} = 10\text{ mV}/^\circ\text{C} = 0.010\text{ V}/^\circ\text{C}$$
At $25^\circ\text{C}$, the LM35 outputs $250\text{ mV}$; at $100^\circ\text{C}$, it outputs $1.00\text{ V}$.
The PIC16F877A ADC samples this analog voltage through pin `AN0`:
$$V_{in} = \frac{\text{ADC} \times 5.0\text{ V}}{1023} \implies T\ (^\circ\text{C}) = \frac{V_{in}}{0.010\text{ V}} = \frac{\text{ADC} \times 500}{1023}$$
To avoid floating-point math overhead on an 8-bit MCU, the firmware utilizes fixed-point scaling:
$$T_{\times 10} = \frac{\text{ADC} \times 5000 + 511}{1023}$$
This yields the temperature scaled by 10 with half-LSB rounding, allowing extraction of hundreds, tens, units, and tenths digits directly via integer arithmetic.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | Microcontroller reading ADC and driving LCD |
| Temperature Sensor | U2 | LM35 Precision Centigrade Sensor | 1 | Linear 10 mV/°C analog temperature transducer |
| Alphanumeric LCD | LCD1 | LM016L (16x2 Character Display) | 1 | Temperature display |
| Crystal Oscillator | X1 | 20 MHz Quartz Crystal | 1 | Master clock reference |
| Resistors | R1, R2 | 10 kOhm, 0.25 W | 2 | Pull-up on MCLR / Contrast bias |
| Potentiometer | RV1 | 1 kOhm linear | 1 | Contrast adjustment for LCD |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 2 | `RA0/AN0` | LM35 Pin 2 (`VOUT`) | Analog temperature input signal |
| Pin 33 | `RB0/INT` | LCD Pin 4 (`RS`) | Register Select line |
| Pin 34 | `RB1` | LCD Pin 6 (`EN`) | Enable strobe line |
| Pins 27–30 | `RD4–RD7` | LCD Pins 11–14 (`D4–D7`) | 4-bit LCD data bus |
| LM35 Pin 1 | `VS` | +5.0 V | Sensor power supply |
| LM35 Pin 3 | `GND` | Ground (0 V) | Sensor ground reference |

## 4. Technical Explanation & Register Configuration
- **`ADCON1 = 0x8E`:** Right-justified result (`ADFM=1`), `AN0` analog, references tied to internal VDD ($5\text{ V}$) and VSS ($0\text{ V}$).
- **`ADCON0 = 0x81`:** Conversion clock $F_{OSC}/32$, Channel 0 enabled, ADC powered on.
- **Fixed-Point Digit Extraction:**
  ```c
  temp = ((unsigned long)adc_value * 5000UL + 511UL) / 1023UL;
  // Hundreds digit: (temp / 1000) + '0'
  // Tens digit:     ((temp / 100) % 10) + '0'
  // Units digit:    ((temp / 10) % 10) + '0'
  // Decimal dot:    '.'
  // Tenths digit:   (temp % 10) + '0'
  ```

## 5. Testing Observations
- Verified in Proteus schematic `Project_08_Temperature_Sensor`.
- Setting LM35 input temperature to $56.0^\circ\text{C}$ generates an ADC conversion that formats and renders on the LCD display as `TEMPERATURE / TEMP: 056.2 C`.
