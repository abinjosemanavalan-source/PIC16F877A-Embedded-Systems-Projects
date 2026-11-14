# Circuit and Technical Logic: Button Debouncing with LCD

## 1. Working Principle
Mechanical switches consist of spring-loaded metal contacts. When actuated, mechanical contacts rebound and chatter repeatedly for 5 to 20 ms before establishing a steady electrical connection. Because a PIC running at 20 MHz executes an instruction every 200 ns, an unconditioned switch causes the microcontroller to detect dozens of false presses for a single physical press.
Software debouncing solves this problem without external RC filter hardware:
1. Detect initial transition (logic LOW).
2. Wait 20 ms (`__delay_ms(20)`) to allow switch contact chatter to subside.
3. Re-verify switch state (`if (BUTTON == 0)`). If confirmed, increment counter.
4. Block on switch release (`while (BUTTON == 0);`) followed by a 20 ms release debounce delay before accepting new events.

## 2. Component List
| Component | Designator | Specification / Rating | Quantity | Purpose |
|---|---|---|---|---|
| Microcontroller | U1 | PIC16F877A-I/P | 1 | CPU executing debounce logic |
| Alphanumeric LCD | LCD1 | LM016L (16x2 Character Display) | 1 | Count visualization |
| Pushbutton | BUTTON | SPST Momentary Tactile Switch | 1 | User counter increment input |
| Pull-up Resistor | R1 | 10 kOhm, 0.25 W | 1 | Ensures clean HIGH state when button is open |
| Contrast Potentiometer | RV1 | 1 kOhm linear | 1 | LCD contrast bias voltage on VEE |

## 3. Pin Connections
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 34 | `RB1` | Pushbutton & R1 (10 kOhm pull-up to +5V) | Active-low counter input |
| Pin 33 | `RB0/INT` | LCD Pin 4 (`RS`) | Register Select (0 = Command, 1 = Data) |
| Pin 35 | `RB2` | LCD Pin 6 (`EN`) | Enable strobe line |
| Pin 27 | `RD4/PSP4` | LCD Pin 11 (`D4`) | 4-bit data bus bit 0 |
| Pin 28 | `RD5/PSP5` | LCD Pin 12 (`D5`) | 4-bit data bus bit 1 |
| Pin 29 | `RD6/PSP6` | LCD Pin 13 (`D6`) | 4-bit data bus bit 2 |
| Pin 30 | `RD7/PSP7` | LCD Pin 14 (`D7`) | 4-bit data bus bit 3 |
| Pin 3 | LCD VEE | RV1 wiper | Contrast voltage adjustment |

## 4. Technical Explanation & Register Configuration
- **`ADCON1 = 0x06`:** Disables all analog inputs on PORTA/PORTE, setting pins as digital I/O.
- **`TRISB = 0b00000010`:** Sets `RB1` as digital input, and `RB0`, `RB2` as digital outputs.
- **`TRISD = 0x0F`:** Sets `RD4-RD7` as digital outputs to drive LCD data lines.
- **LCD Nibble Multiplexing:**
  Commands and characters are split into high and low 4-bit nibbles:
  `PORTD = (PORTD & 0x0F) | (data & 0xF0);` followed by an enable pulse (`EN=1; __delay_ms(2); EN=0;`).
- **Debounce Logic:**
  ```c
  if (BUTTON == 0) {
      __delay_ms(20);          // Filter make bounce
      if (BUTTON == 0) {
          count++;
          Display_Count(count);
          while (BUTTON == 0); // Wait for release
          __delay_ms(20);      // Filter break bounce
      }
  }
  ```

## 5. Testing Observations
- Verified in Proteus schematic `PROJECT_2 Button_Debouncing_with_LCD`.
- LCD reliably increments by exactly 1 on every tactile click (`Count: 001`, `Count: 002`, ..., `Count: 005`).
- Rapid chatter and false multi-triggering are 100% eliminated.
