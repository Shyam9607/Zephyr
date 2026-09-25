# Air Mouse Hardware Specification

**Project:** Personal ESP32 Air Mouse  
**Document:** Hardware Specification  
**Version:** 0.1.0  
**Status:** Draft

---

# 1. Purpose

This document defines the hardware used or planned for the Air Mouse prototype and establishes hardware-related constraints that firmware and future PCB designs must respect.

This document is the hardware source of truth for the project.

Hardware specifications MUST be verified against the relevant datasheets before implementation.

---

# 2. Current Hardware

## 2.1 Main Controller

### ESP32 DevKit-style Board

The physical board supplied for the project is an ESP32 DevKit-style development board.

The installed module is visibly marked:

`ESP-32`

and identifies itself as:

`WiFi + Bluetooth`

with the radio specification:

`802.11 b/g/n`

The board appears to use the original ESP32-WROOM-32 family.

### Current status

- Controller: ESP32
- Module family: ESP32-WROOM-32
- Board type: ESP32 DevKit-style development board
- USB programming interface: Present
- BOOT button: Present
- EN/reset button: Present
- Logic voltage: 3.3 V
- Bluetooth: Available
- Wi-Fi: Available

### Important

The exact third-party DevKit board revision, USB-UART bridge, voltage regulator, and board-level power circuitry MUST be verified before making board-specific electrical assumptions.

The ESP32-WROOM-32 documentation and the actual physical board take precedence over assumptions based on the generic name "ESP32 DevKit".

---

# 3. ESP32 Requirements

The ESP32 is responsible for:

- Sensor acquisition
- Motion processing
- Button/encoder processing
- Bluetooth HID
- Configuration storage
- Profile management
- Diagnostics
- Power management where supported by the final hardware

The firmware should use the ESP32's hardware peripherals rather than implementing unnecessary software replacements.

Potentially relevant peripherals include:

- GPIO
- I2C
- SPI
- UART
- Hardware timers
- Interrupts
- PWM
- ADC
- Bluetooth
- Non-volatile storage

Only peripherals required by the final design should be enabled.

---

# 4. Motion Sensor

## 4.1 MPU6050

The initial motion-sensor candidate is the MPU6050.

The MPU6050 provides:

- 3-axis accelerometer
- 3-axis gyroscope
- I2C interface

### Intended use

The gyroscope is expected to be the primary sensor for cursor movement.

The accelerometer may be used for:

- Orientation estimation
- Motion detection
- Calibration
- Stabilization
- Future gesture features

### Hardware interface

Target interface:

`ESP32 ↔ I2C ↔ MPU6050`

The exact ESP32 I2C pins are TBD and MUST be finalized after reviewing the board pinout and hardware constraints.

The MPU6050 I2C address and address-selection configuration MUST be verified from the actual module and datasheet.

---

# 5. Scroll / Navigation Control

## 5.1 Primary Candidate

The current planned scroll control is an Adafruit ANO-style directional navigation and scroll wheel.

The intended device provides:

- Rotary encoder
- Up button
- Down button
- Left button
- Right button
- Center button

### Intended use

The rotary encoder will primarily provide vertical scrolling.

The additional directional inputs may later be used for:

- Page navigation
- Horizontal scrolling
- Media controls
- Profile switching
- Recenter
- Keyboard shortcuts
- Custom actions

The exact component and interface MUST be confirmed before PCB design.

---

# 6. Primary Mouse Buttons

The initial physical design contains three primary buttons:

```text
L     M     R
```

Default functions:

- L = Left mouse button
- M = Middle mouse button
- R = Right mouse button

The buttons should be physically positioned so that they can be operated naturally while holding the air mouse.

Exact switch technology, dimensions, actuation force, and mounting method are TBD.

---

# 7. Physical Layout

Current conceptual layout:

```text
             ┌───┐ ┌───┐ ┌───┐
             │ L │ │ M │ │ R │
             └───┘ └───┘ └───┘


                    ◯
                  SCROLL


              AIR MOUSE BODY
```

Design goals:

- Compact handheld form factor
- Three primary buttons near the front/top
- Scroll control reachable by the thumb
- Stable grip during motion
- Sensor located in a mechanically meaningful position
- Minimal unnecessary physical controls

The final mechanical geometry is not yet defined.

---

# 8. Sensor Placement

The MPU6050 should be placed in a mechanically stable location.

Preferred concept:

- Near the central area of the device
- Firmly attached to the PCB
- Orientation clearly documented
- Minimal mechanical movement relative to the enclosure

The sensor's physical X/Y/Z axes MUST be documented once the PCB orientation is finalized.

The firmware MUST NOT rely on undocumented assumptions about sensor orientation.

---

# 9. Electrical Interface

## 9.1 I2C

The MPU6050 will use I2C.

Expected signals:

```text
ESP32
 │
 ├── SDA ───── MPU6050 SDA
 │
 └── SCL ───── MPU6050 SCL
```

Additional lines such as:

- INT
- AD0

may be used if required.

Exact GPIO assignments are TBD.

---

# 10. GPIO Planning

GPIO assignments MUST NOT be permanently selected until:

1. The exact ESP32 board pinout is verified.
2. Required boot-strapping pins are identified.
3. USB/UART pins are considered.
4. I2C pins are selected.
5. Encoder/button requirements are mapped.
6. Future PCB requirements are considered.

The final mapping will be recorded in this document.

---

# 11. Interrupts

Hardware interrupts MAY be used for:

- Encoder transitions
- Button events
- MPU6050 interrupt
- Timed firmware events

Interrupt handlers MUST remain short and should not perform heavy processing.

Sensor processing and HID generation should occur outside interrupt handlers where practical.

---

# 12. Power

## Prototype

The first prototype will use USB power from the ESP32 development board.

## Portable Revision

A future portable revision may include:

- 3.7 V Li-ion/LiPo battery
- Charging circuit
- Battery protection
- Power switch
- Voltage regulation
- Battery monitoring
- Low-power sleep

Battery architecture is intentionally not finalized yet.

### Safety rule

A Li-ion/LiPo battery MUST NOT be connected directly to an ESP32 3.3 V rail unless the complete power architecture explicitly supports the voltage and current requirements.

The final power system must be designed around the actual regulator/charging circuitry.

---

# 13. USB

The development board provides USB connectivity for:

- Firmware flashing
- Serial communication
- Development diagnostics

The USB connector is required for the prototype.

The final custom PCB may use USB-C, subject to the final electrical design.

---

# 14. Bluetooth

The ESP32 will provide wireless connectivity.

Target:

`Bluetooth HID`

The finished device should present itself as a standard mouse/input device to the host computer wherever practical.

The Bluetooth implementation must be selected based on the capabilities and limitations of the exact ESP32 firmware stack used by the project.

---

# 15. Indicators

Potential indicators include:

- Power
- Bluetooth connection
- Pairing
- Active profile
- Calibration
- Recenter
- Low battery
- Error state

No final LED configuration has been selected.

Indicators must not compromise ergonomics or unnecessarily increase power consumption.

---

# 16. Future PCB

The prototype will use the development board and breakout modules.

After the prototype is validated, a custom PCB may integrate:

- ESP32 module
- MPU6050 or improved IMU
- Scroll encoder
- Primary buttons
- Additional controls
- Power circuitry
- Charging circuitry
- USB
- LEDs
- Test/debug access

The custom PCB MUST NOT be designed as the first implementation target.

The prototype must first validate:

- Electrical connections
- Sensor behavior
- Motion algorithm
- Control layout
- HID behavior
- Ergonomics

---

# 17. Future Enclosure

The final enclosure will be designed around the validated hardware and control layout.

Requirements include:

- Comfortable handheld grip
- Thumb access to scroll control
- Finger access to L/M/R
- Secure PCB mounting
- Stable IMU mounting
- USB access
- Power-switch access if battery powered
- Appropriate ventilation/thermal considerations
- No mechanical interference with buttons or encoder

The enclosure may initially be 3D printed.

---

# 18. Hardware Rules

The following rules apply to all agents and developers.

1. Never invent GPIO assignments.
2. Never assume an ESP32 board revision from the generic board name alone.
3. Never assume a component's voltage requirements.
4. Never connect a battery directly to a rail without verifying the power architecture.
5. Never rely on an unverified datasheet value.
6. Hardware changes MUST be documented.
7. GPIO changes MUST be reflected in the hardware documentation.
8. Sensor orientation MUST be explicitly documented.
9. Prototype wiring MUST be validated before PCB design.
10. Firmware MUST remain adaptable to finalized hardware pin assignments.

---

# 19. Datasheet Sources

The project should keep the relevant primary documentation under:

```text
datasheets/
├── esp32/
├── mpu6050/
├── encoder/
└── other/
```

Important ESP32 documentation includes:

- ESP32-WROOM-32 Datasheet
- ESP32 Technical Reference Manual
- ESP32 hardware design documentation
- Relevant development-board documentation

The exact MPU6050 datasheet and selected encoder documentation should be added before implementing hardware-specific drivers.

---

# 20. Current Hardware Status

## Controller

- [x] ESP32 DevKit-style board obtained
- [x] ESP32 module identified
- [ ] Exact board revision verified
- [ ] Exact USB-UART bridge verified
- [ ] Board regulator verified
- [ ] Final GPIO map defined

## Motion

- [ ] MPU6050 acquired
- [ ] MPU6050 module identified
- [ ] I2C wiring defined
- [ ] Sensor orientation defined
- [ ] Interrupt strategy defined

## Controls

- [ ] L/M/R switches selected
- [ ] Scroll encoder selected
- [ ] Scroll encoder interface verified
- [ ] Recenter control finalized

## Power

- [x] USB-powered prototype planned
- [ ] Battery selected
- [ ] Charging circuit selected
- [ ] Protection strategy selected
- [ ] Portable power architecture finalized

## Mechanical

- [ ] Physical dimensions
- [ ] PCB dimensions
- [ ] Enclosure
- [ ] Grip geometry
- [ ] Button geometry
- [ ] Scroll-wheel mounting

---

# 21. Hardware Definition of Done

The prototype hardware is considered ready for firmware integration when:

1. ESP32 can be programmed reliably.
2. ESP32 power rails are verified.
3. MPU6050 can communicate over I2C.
4. Sensor orientation is documented.
5. L/M/R buttons can be read reliably.
6. Scroll input can be read reliably.
7. All GPIO assignments are documented.
8. No component exceeds its electrical limits.
9. The prototype can operate continuously without unexpected resets.
10. Hardware-specific assumptions are backed by documentation or measurement.

---

# 22. Open Hardware Decisions

The following remain intentionally unresolved:

- Exact MPU6050 breakout/module
- Exact scroll encoder
- ANO direct-GPIO vs I2C adapter
- Button switch type
- GPIO allocation
- MPU6050 interrupt usage
- Battery type
- Charging circuit
- Power switch
- LED configuration
- USB implementation for custom PCB
- Final PCB dimensions
- Final enclosure dimensions
- Potential future IMU upgrade
