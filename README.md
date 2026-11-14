# PIC16F877A Embedded Systems Projects

**A Practical Embedded Systems Portfolio | MPLAB X IDE | XC8 Compiler | Proteus Design Suite**

A collection of 10 PIC16F877A firmware and circuit-design exercises exploring microcontroller programming, peripherals, interrupts, timers, ADC, sensors and PWM.

This repository organizes the supplied project files into individual project folders containing MPLAB sources, Proteus designs, images and available documentation.

---




## About the PIC16F877A

The **PIC16F877A** is an 8-bit microcontroller from Microchip used in this collection to explore fundamental embedded systems concepts.

The projects cover:

- Digital input and output (GPIO)
- External interrupts
- Timer operations
- Watchdog Timer
- 10-bit Analog-to-Digital Conversion (ADC)
- Analog sensor interfacing
- PWM concepts

Exact device operating conditions and electrical limits must be checked against the device datasheet before hardware use.

---

## Technologies & Tools

| Technology | Purpose |
|---|---|
| PIC16F877A | Microcontroller |
| Embedded C | Firmware development |
| MPLAB X IDE | Project development |
| XC8 Compiler | C compilation |
| Proteus Design Suite | Circuit design and simulation |
| Git & GitHub | Version control and project hosting |



---

## Project Index

The repository contains 10 numbered projects.

| No. | Project | Main Concept |
|:---:|---|---|
| 01 | [Watchdog Timer](01-Watchdog-Timer/README.md) | Automatic firmware recovery |
| 02 | [Button Debouncing](02-Button-Debouncing/README.md) | Switch debouncing and LCD |
| 03 | [External Interrupt](03-External-Interrupt/README.md) | RB0/INT interrupt handling |
| 04 | [Timer0 LED Scheduling](04-Timer0-LED-Scheduling/README.md) | Timer-based scheduling |
| 05 | [Floating Input](05-Floating-Input/README.md) | Pull-up and pull-down resistors |
| 06 | [ADC Potentiometer](06-ADC-Potentiometer/README.md) | 10-bit ADC conversion |
| 07 | [Digital Voltmeter](07-Digital-Voltmeter/README.md) | ADC-based voltage measurement |
| 08 | [LM35 Temperature Sensor](08-LM35-Temperature-Sensor/README.md) | Temperature measurement |
| 09 | [LDR Automatic Light](09-LDR-Automatic-Light/README.md) | Light sensing and hysteresis |
| 10 | [CCP PWM LED Dimming](10-CCP-PWM-LED-Dimming/README.md) | Hardware PWM LED control |

### Project Implementation Status

| Project | Available Files | Verification Status |
|---|---|---|
| 01 – Watchdog Timer | Source, MPLAB and Proteus | Not independently tested |
| 02 – Button Debouncing | Source, MPLAB and Proteus | Not independently tested |
| 03 – External Interrupt | Source, MPLAB and Proteus | Not independently tested |
| 04 – Timer0 Scheduling | Source, MPLAB and Proteus | Delay-comparison implementation not identified |
| 05 – Floating Input | Source, MPLAB and Proteus | Circuit biasing needs confirmation |
| 06 – ADC Potentiometer | Source, MPLAB and Proteus | Wiring needs verification |
| 07 – Digital Voltmeter | Source and Proteus variants | No HEX in source project |
| 08 – LM35 Temperature | Matching MPLAB and Proteus variants | Not independently tested |
| 09 – LDR Automatic Light | Source, MPLAB and Proteus | Direction and wiring need verification |
| 10 – CCP PWM | Project documentation | Matching implementation not identified |



---

## Screenshots & Images

The supplied numbered PNG files have been copied into the respective project `Images/` folders as:

`circuit_simulation.png`

These are original supplied assets, not newly generated captures.

Their contents, provenance and relationship to the current Proteus files have not been independently verified. Therefore, treat them as reference images rather than proof of successful simulation.

To inspect the images, navigate to the corresponding project's `Images/` directory.

---

## Skills Represented by the Available Files

The available project files provide examples of the following embedded systems concepts:

- **Embedded C Programming** – Microcontroller firmware structure.
- **GPIO Configuration** – Digital input and output configuration.
- **Interrupt Handling** – External interrupt service routines and event flags.
- **Timer Programming** – Timer0 overflow interrupts and foreground event processing.
- **Watchdog Timer** – Watchdog configuration and servicing.
- **ADC Conversion** – Analog input acquisition and basic scaling.
- **LCD Interfacing** – LCD helper routines and formatted numeric output.
- **Sensor Interfacing** – Analog sensor input and threshold-based control.
- **Development Tools** – MPLAB X IDE and Proteus project organization.

---

## Learning Outcomes

This collection provides a foundation for understanding and practicing:

1. Microcontroller peripheral configuration.
2. Embedded C programming structures.
3. Interrupt and main-loop interaction.
4. Timer-based event scheduling.
5. Analog-to-digital conversion.
6. Sensor data processing.
7. LCD interfacing and numeric display.
8. Proteus circuit design and simulation workflows.

Each circuit and firmware build should be independently verified before using the repository as evidence of tested behavior.

---

## Future Improvements

The following improvements are planned:

- [ ] Identify or add the actual CCP PWM project and verify its mapping.
- [ ] Open every Proteus project and verify dependencies and firmware links.
- [ ] Capture genuine Proteus simulation screenshots.
- [ ] Rebuild each MPLAB project using documented MPLAB X and XC8 versions.
- [ ] Record successful compilation and simulation results.
- [ ] Confirm pin assignments and electrical limits against schematics and datasheets.
- [ ] Add component values and detailed circuit diagrams.
- [ ] Document actual testing observations.
- [ ] Resolve the duplicate LM35 project mapping after reviewing the original files.

---

## Repository Structure

```text
PIC16F877A-Embedded-Systems-Projects/
│
├── 01-Watchdog-Timer/
├── 02-Button-Debouncing/
├── 03-External-Interrupt/
├── 04-Timer0-LED-Scheduling/
├── 05-Floating-Input/
├── 06-ADC-Potentiometer/
├── 07-Digital-Voltmeter/
├── 08-LM35-Temperature-Sensor/
├── 09-LDR-Automatic-Light/
├── 10-CCP-PWM-LED-Dimming/
│
├── _REVIEW/
│   ├── Alternative-Implementations/
│   └── Original-Workspace-Backups/
│
├── ORGANIZATION_REPORT.md
├── .gitignore
└── README.md
```

### Individual Project Folder Structure

Each numbered project follows this general organization:

```text
Project-Name/
│
├── MPLAB/
│   └── Source code and IDE project files
│
├── Proteus/
│   └── Circuit design and simulation files
│
├── Images/
│   └── Supplied project images
│
├── Documentation/
│   └── Technical notes, where available
│
└── README.md
```

Loose source folders found in the original workspace are preserved under `_REVIEW/Original-Workspace-Backups/`.



---

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/abinjosemanavalan-source/PIC16F877A-Embedded-Systems-Projects.git
```

### 2. Open a Project in MPLAB X IDE

1. Launch MPLAB X IDE.
2. Select **File → Open Project**.
3. Navigate to the required project's `MPLAB/` folder.
4. Select the MPLAB X project.

### 3. Compile the Firmware

1. Ensure the XC8 compiler is installed and configured.
2. Select the appropriate project configuration.
3. Build the project.
4. Review compiler output for errors and warnings.

### 4. Open the Proteus Simulation

1. Launch Proteus Design Suite.
2. Open the relevant `.pdsprj` file.
3. Verify component connections and firmware paths.
4. Load the compiled HEX file if required.
5. Run the simulation.

*Actual project compatibility and simulation results must be verified individually.*

---

## Author

**Abin Jose**

Embedded Systems | Artificial Intelligence | Machine Learning | Robotics

- GitHub: [@abinjosemanavalan-source](https://github.com/abinjosemanavalan-source)

---

*This repository represents an ongoing practical learning journey in embedded systems engineering. Project implementations, documentation and verification will continue to improve as development progresses.*
