# 10. CCP PWM LED Dimming

## 1. Project Title
**Hardware Pulse Width Modulation (PWM) LED Dimming Using PIC16F877A CCP1 Module**

## 2. Project Overview
Use the PIC CCP hardware PWM module to control LED brightness by changing the PWM duty cycle.

## 3. Objective
- Understand the hardware architecture of the PIC16F877A Capture/Compare/PWM (`CCP1`) peripheral.
- Derive period register `PR2` and prescaler calculations for a target PWM switching frequency.
- Program 10-bit PWM duty cycles using `CCPR1L` (8 MSBs) and `CCP1CON<5:4>` (2 LSBs).
- Demonstrate smooth, flicker-free LED brightness modulation without CPU execution overhead.

## 4. Components Required
- **Microcontroller:** Microchip PIC16F877A
- **Actuator:** 5 mm High-Brightness LED
- **Resistor:** 1x 330 Ohm, 0.25 W (current limiting)
- **Clock:** 20 MHz Quartz Crystal with dual 22 pF load capacitors
- **Power Supply:** +5.0 V DC & Ground

## 5. Software and Tools
- **IDE:** MPLAB X IDE
- **Compiler:** Microchip XC8 Compiler
- **Simulation Suite:** Proteus 8 Professional
- **Target Device:** PIC16F877A

## 6. Circuit Connections
| Pin # | PIC16F877A Pin | Component Connection | Purpose |
|---|---|---|---|
| Pin 17 | `RC2/CCP1` | 330 Ohm -> Anode of Dimming LED | Hardware PWM output pin |
| Pins 13, 14 | `OSC1, OSC2` | 20 MHz Quartz Crystal | High-speed clock input |
| Pins 11, 32 | `VDD` | +5.0 V | Power supply rail |
| Pins 12, 31 | `VSS` | GND (0 V) | Ground reference |

## 7. Working Principle
Pulse Width Modulation (PWM) varies the average power delivered to a load by rapidly switching voltage between $0\text{ V}$ and $5\text{ V}$ at a fixed frequency:
$$V_{avg} = V_{DD} \times \frac{T_{ON}}{T_{PWM}} = 5\text{ V} \times \text{Duty Cycle}$$
At frequencies above 100 Hz, the human eye averages the photonic emission without perceiving flicker.
The PIC16F877A CCP1 module uses hardware **Timer2** to generate the time base:
- **PWM Period:**
  $$T_{PWM} = [(PR2) + 1] \times 4 \times T_{OSC} \times (\text{TMR2 Prescaler})$$
  With $F_{OSC} = 20\text{ MHz}$ ($T_{OSC} = 50\text{ ns}$), Timer2 Prescaler = 16, and $PR2 = 255$:
  $$T_{PWM} = 256 \times 4 \times 50\text{ ns} \times 16 = 819.2\ \mu\text{s} \implies F_{PWM} \approx 1.22\text{ kHz}$$
- **10-Bit Duty Cycle:**
  The 10-bit duty cycle count spans $0$ (0% duty cycle, LED OFF) to $1023$ (100% duty cycle, LED full brightness). The 8 MSBs reside in `CCPR1L`, and the 2 LSBs reside in `CCP1CON<5:4>`.

## 8. Code Explanation
Detailed architecture provided in [`Documentation/PWM_SPECIFICATION.md`](Documentation/PWM_SPECIFICATION.md):
- **Step 1:** Configure `TRISCbits.TRISC2 = 1` during setup.
- **Step 2:** Load period register `PR2 = 255`.
- **Step 3:** Set PWM mode: `CCP1CON = 0x0C` (bits 3:0 = `1100`).
- **Step 4:** Set initial duty cycle: `CCPR1L = 0; CCP1CONbits.CCP1X = 0; CCP1CONbits.CCP1Y = 0;`.
- **Step 5:** Start Timer2: `T2CON = 0b00000110` (Prescaler 1:16, Timer2 ON).
- **Step 6:** Enable output: `TRISCbits.TRISC2 = 0`.
- **Step 7:** Incrementally sweep duty cycle in `while(1)` to smoothly fade the LED.

## 9. How to Run the Proteus Simulation
*TODO: Pending user commit of the dedicated CCP PWM simulation schematic (`.pdsprj`) and firmware project.*
1. Load the compiled PWM firmware into PIC16F877A (U1).
2. Connect an oscilloscope probe to pin 17 (`RC2/CCP1`) to observe the 1.22 kHz square wave.
3. Observe pulse width modulation and corresponding LED luminous intensity change.

## 10. Expected Output
- Oscilloscope displays a stable 1.22 kHz PWM waveform on pin `RC2`.
- Duty cycle sweeps progressively from 0% to 100%, causing the LED to smoothly brighten and dim.

## 11. Actual Testing Status
- **Implementation Status:** ⚠️ **TODO: Pending Firmware Commit**.
- **Portfolio Note:** In accordance with portfolio integrity rules, no imaginary source code or fake simulation tests have been fabricated. Complete register calculations, formulas, and pin mappings are documented in [`Documentation/PWM_SPECIFICATION.md`](Documentation/PWM_SPECIFICATION.md).
- *(Note: The original workspace folder `Project_10.X` contained an alternative LM35 temperature monitoring design utilizing PORTB for LCD control; that design has been safely preserved under `_REVIEW/Alternative-Implementations/Project_10-LM35-PortB/`).*

## Screenshots

### Proteus Circuit Simulation

**Status:** Simulation screenshot pending.

The Proteus circuit and firmware implementation have not yet been committed to this project.

The following screenshots will be added after implementation and verification:

- Proteus circuit schematic.
- PWM output waveform captured using the oscilloscope.
- LED brightness variation during the duty-cycle sweep.

**Planned image files:**

Images/circuit_simulation.png



## 13. Learning Outcomes
- Understanding hardware PWM generation via dedicated microcontroller peripherals.
- Calculating Timer2 prescaler and `PR2` period parameters.
- Programming 10-bit resolution duty cycles across multi-byte register boundaries.
- Offloading periodic waveform generation from CPU core to peripheral silicon.
