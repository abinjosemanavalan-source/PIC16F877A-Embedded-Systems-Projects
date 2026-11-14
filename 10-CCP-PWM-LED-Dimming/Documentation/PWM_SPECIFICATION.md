# Technical Specification & Register Architecture: CCP PWM LED Dimming

## 1. Project Scope & Architecture
This document provides the hardware specification, register calculations, and software roadmap for implementing Pulse Width Modulation (PWM) on the PIC16F877A microcontroller using the on-chip **Capture/Compare/PWM 1 (`CCP1`)** peripheral.

## 2. Working Principle
Pulse Width Modulation (PWM) controls the average electrical power delivered to a load by rapidly switching power ON and OFF at a fixed frequency while varying the ratio of ON time to total period (the **Duty Cycle**):
$$\text{Duty Cycle (\%)} = \frac{T_{ON}}{T_{PWM}} \times 100\%$$
$$\text{Average Voltage } V_{avg} = V_{DD} \times \frac{T_{ON}}{T_{PWM}}$$
Because the switching frequency is significantly higher than the human eye's flicker fusion threshold (> 100 Hz), the human eye perceives variation in duty cycle as smooth, flicker-free LED dimming.

## 3. Register Architecture & Mathematical Derivation
The PIC16F877A CCP1 PWM module is driven by internal hardware **Timer2 (`TMR2`)**.

### 3.1 PWM Period Calculation
The PWM period is defined by the value written into the 8-bit period register **`PR2`**:
$$T_{PWM} = [(PR2) + 1] \times 4 \times T_{OSC} \times (\text{TMR2 Prescaler})$$
$$F_{PWM} = \frac{1}{T_{PWM}} = \frac{F_{OSC}}{4 \times [(PR2) + 1] \times (\text{TMR2 Prescaler})}$$

For a standard **20 MHz crystal** ($F_{OSC} = 20\text{ MHz} \implies T_{OSC} = 50\text{ ns}$):
- Selecting **Timer2 Prescaler = 16** (`T2CONbits.T2CKPS = 0b10`):
- Setting **`PR2 = 255`** (maximum 8-bit resolution):
$$T_{PWM} = [255 + 1] \times 4 \times (50\text{ ns}) \times 16 = 256 \times 200\text{ ns} \times 16 = 819.2\ \mu\text{s}$$
$$F_{PWM} = \frac{1}{819.2\ \mu\text{s}} \approx 1.2207\text{ kHz} \approx 1.22\text{ kHz}$$

### 3.2 PWM Duty Cycle Calculation
The PWM duty cycle is defined with **10-bit resolution**:
- **8 Most Significant Bits (MSBs):** Written into register **`CCPR1L`**.
- **2 Least Significant Bits (LSBs):** Written into bits **`CCP1CON<5:4>`** (`CCP1X` and `CCP1Y`).
$$\text{PWM Duty Cycle Time } T_{ON} = (CCPR1L:CCP1CON<5:4>) \times T_{OSC} \times (\text{TMR2 Prescaler})$$
$$\text{Total 10-bit Count for 100\% Duty Cycle} = 4 \times (PR2 + 1) = 4 \times 256 = 1024\text{ counts}$$

| Target Duty Cycle | 10-bit Decimal Value | `CCPR1L` (8 MSBs) | `CCP1CON<5:4>` (2 LSBs) | Output Average Voltage ($V_{DD} = 5\text{V}$) |
|---|---|---|---|---|
| **0% (OFF)** | `0` | `0x00` | `0b00` | $0.00\text{ V}$ |
| **25%** | `256` | `0x40` (`64`) | `0b00` | $1.25\text{ V}$ |
| **50%** | `512` | `0x80` (`128`) | `0b00` | $2.50\text{ V}$ |
| **75%** | `768` | `0xC0` (`192`) | `0b00` | $3.75\text{ V}$ |
| **100% (Full ON)** | `1023` | `0xFF` (`255`) | `0b11` | $5.00\text{ V}$ |

## 4. Hardware Pin Mapping & Circuit Layout
| Pin Number | PIC16F877A Pin Name | Connected To | Function / Description |
|---|---|---|---|
| Pin 17 | `RC2/CCP1` | Series Resistor ($330\ \Omega$) -> Anode of Dimming LED | Hardware PWM output pin |
| Pins 13, 14 | `OSC1, OSC2` | 20 MHz Quartz Crystal & two 22 pF ceramic caps | Master system clock |
| Pins 11, 32 | `VDD` | +5.0 V DC | Power supply rail |
| Pins 12, 31 | `VSS` | GND (0 V) | Ground reference |

## 5. Implementation Roadmap (TODO)
When committing firmware for this project:
1. Set `TRISCbits.TRISC2 = 1` (temporarily disable output driver during setup).
2. Write period register: `PR2 = 255`.
3. Configure `CCP1CON = 0x0C` (PWM mode: bits 3:0 = `1100`).
4. Set initial duty cycle: `CCPR1L = 0; CCP1CONbits.CCP1X = 0; CCP1CONbits.CCP1Y = 0;`.
5. Configure Timer2: `T2CON = 0b00000110` (Prescaler 1:16, Timer2 ON).
6. Enable output driver: `TRISCbits.TRISC2 = 0`.
7. Sweep duty cycle inside `while(1)` with a small delay to demonstrate smooth LED fading.
