# Air Mouse
## Product Requirements Document

**Project:** Personal ESP32 Air Mouse  
**Document:** Product Requirements Document  
**Version:** 0.1.0  
**Status:** Draft  
**Platform:** ESP32-WROOM-32 based DevKit  
**Primary Sensor:** MPU6050  
**Primary Connectivity:** Bluetooth HID  
**Development Environment:** VS Code + PlatformIO  
**Target User:** Single-user personal device

---

# 1. Product Overview

The Air Mouse is a custom-built wireless pointing device based on an ESP32.

The device will use an IMU to detect hand/wrist movement and translate that movement into mouse cursor movement on a computer.

The device will also provide physical controls for conventional mouse functions, scrolling, profile switching, and potentially additional user-defined functions.

The long-term goal is to create a highly customizable personal input device with functionality comparable to advanced/gaming mice while keeping the configuration and behavior under the user's direct control through firmware.

The device should operate as a standalone Bluetooth HID peripheral and should not require dedicated proprietary desktop software for normal operation.

---

# 2. Product Goals

## 2.1 Primary Goals

The device MUST:

1. Function as a wireless mouse through Bluetooth HID.
2. Translate IMU motion into cursor movement.
3. Provide physical mouse controls.
4. Provide a dedicated scrolling mechanism.
5. Support configurable button functions.
6. Support configurable cursor sensitivity.
7. Support sensor calibration.
8. Support cursor recentering.
9. Store persistent configuration on the ESP32.
10. Operate without requiring a dedicated desktop configuration application.
11. Be developed using a maintainable modular firmware architecture.
12. Be testable incrementally on physical hardware.

---

# 3. Design Philosophy

The project is intended to be:

- Modular
- Maintainable
- Highly configurable
- Hardware-aware
- Firmware-controlled
- Expandable
- Personal-use focused
- Independent of proprietary configuration software

The device should prioritize user control and predictable behavior over unnecessary complexity.

Features should be implemented only when they provide a clear benefit to the device.

---

# 4. Initial Hardware

## 4.1 Main Controller

Current controller:

- ESP32-WROOM-32 based DevKit board

The exact board-level pinout, USB-UART implementation, regulator, and power circuitry MUST be verified from the physical board and relevant documentation before final hardware decisions are made.

---

## 4.2 Motion Sensor

Primary candidate:

- MPU6050

The MPU6050 provides:

- 3-axis gyroscope
- 3-axis accelerometer
- I2C interface

The gyroscope is expected to be the primary source for cursor movement.

The accelerometer may be used for:

- Orientation estimation
- Motion detection
- Stabilization
- Calibration assistance
- Future motion features

---

## 4.3 Scroll/Input Control

Primary candidate:

- Adafruit ANO Directional Navigation and Scroll Wheel

Expected functionality:

- Rotary encoder
- Up
- Down
- Left
- Right
- Center button

The exact component and electrical interface remain subject to availability and final hardware design.

---

# 5. User Interaction

## 5.1 Cursor Movement

The user's hand movement should control the cursor without requiring a physical surface.

The motion pipeline should conceptually be:

IMU
→ Sensor Driver
→ Calibration
→ Filtering
→ Motion Processing
→ Cursor Mapping
→ HID Mouse Report
→ Computer

---

## 5.2 Cursor Control Requirements

The firmware SHOULD support:

- Adjustable sensitivity
- X/Y sensitivity adjustment
- Dead zone
- Motion smoothing
- Motion acceleration
- Configurable response curve
- Cursor recentering
- Motion enable/disable
- Configurable motion axis mapping

The exact algorithms will be determined during firmware design and testing.

---

# 6. Re-centering

The device MUST provide a way to establish the current physical orientation as the neutral/reference position.

When the user activates recentering:

1. Current sensor orientation/state is captured.
2. The current orientation becomes the new reference.
3. Subsequent movement is calculated relative to that reference.
4. Re-centering should not require restarting the device.

The physical control used for recentering is TBD.

---

# 7. Mouse Controls

The device SHOULD provide at minimum:

- Left click
- Right click
- Middle click
- Scroll
- Recenter

Additional controls may be added depending on physical layout.

The final physical control layout is TBD.

---

# 8. Scroll Wheel

The primary scroll control is planned to be positioned near the thumb for ergonomic access.

The intended physical layout is:

- Scroll wheel located to the left of the left-click control from the user's perspective.
- Wheel should be reachable naturally by the thumb.
- Rotation should provide vertical scrolling.
- Scroll sensitivity should be configurable.

Potential additional functions:

- Horizontal scrolling
- Scroll acceleration
- Alternate wheel modes
- User-defined wheel actions

These features are subject to hardware and firmware feasibility.

---

# 9. ANO Directional Controls

If the ANO directional encoder is used, its additional controls may be assigned to functions such as:

- Page navigation
- Horizontal scrolling
- Media controls
- Profile switching
- Keyboard shortcuts
- Application-specific actions
- User-defined functions

The final mapping is TBD.

The firmware MUST allow the mapping to be changed without requiring a complete firmware rewrite.

---

# 10. Button Customization

The firmware SHOULD support configurable button actions.

Possible actions include:

- Mouse buttons
- Keyboard keys
- Keyboard combinations
- Media controls
- Scroll actions
- Profile switching
- Recenter
- Custom firmware functions

Button behavior may support:

- Single press
- Long press
- Double press
- Hold
- Release

The exact supported interaction types will be determined during firmware design.

---

# 11. Profiles

The device SHOULD support multiple configuration profiles.

Example:

### Profile 1
General desktop use

### Profile 2
Programming

### Profile 3
Gaming

### Profile 4
Media

Profiles should be stored persistently on the ESP32.

Each profile may contain:

- Cursor sensitivity
- X/Y sensitivity
- Dead zone
- Smoothing
- Acceleration
- Scroll sensitivity
- Button mappings
- Wheel mappings
- Other configurable behavior

The number of profiles is TBD.

---

# 12. Persistent Configuration

Configuration SHOULD survive:

- Power off
- Battery replacement where applicable
- ESP32 restart

Configuration may be stored using ESP32 non-volatile storage.

The firmware MUST protect stored configuration from corruption where practical.

Default configuration MUST be available if stored configuration is invalid or missing.

---

# 13. Bluetooth

The device MUST communicate with the computer wirelessly.

Target protocol:

- Bluetooth HID

The device should appear to the computer as a standard input device wherever practical.

The project should avoid requiring custom desktop drivers for basic mouse functionality.

---

# 14. Power

The initial prototype will operate from USB power.

A future portable version SHOULD support battery operation.

Potential future power system:

- Rechargeable Li-ion/LiPo battery
- Charging circuit
- Protection
- Power switch
- Battery monitoring
- Low-power/sleep behavior

Battery architecture is NOT finalized in the current PRD.

---

# 15. Indicators

The device MAY include LEDs for:

- Power
- Bluetooth connection
- Pairing
- Active profile
- Calibration
- Low battery
- Configuration mode
- Error indication

LED behavior is TBD.

Indicators should not unnecessarily drain battery power in the final portable version.

---

# 16. Calibration

The device MUST support IMU calibration.

Calibration should account for sensor bias and other measurable offsets where practical.

The calibration system SHOULD support:

- Initial calibration
- Gyroscope bias calibration
- Recalibration
- Re-centering

Calibration behavior should be clearly communicated to the user through an indicator where possible.

---

# 17. Motion Processing

The firmware SHOULD separate sensor acquisition from motion processing.

Conceptual architecture:

Sensor acquisition
→ Calibration
→ Filtering
→ Motion estimation
→ Cursor mapping
→ HID output

Motion processing MUST be designed to minimize latency.

The firmware SHOULD avoid unnecessary blocking operations in the motion-processing path.

---

# 18. Latency

The device SHOULD provide responsive cursor movement suitable for normal desktop interaction.

The system should minimize:

- Sensor sampling delays
- Processing delays
- HID transmission delays
- Blocking operations

Latency will be measured and optimized during hardware testing.

No exact latency target is defined yet.

---

# 19. Configuration Without Desktop Software

The core device MUST remain functional without dedicated Windows/macOS/Linux configuration software.

Configuration may eventually be performed through:

- Firmware-defined controls
- On-device configuration mode
- Serial interface during development
- Optional future configuration software

A desktop configuration application is considered optional and is NOT part of the initial product requirement.

---

# 20. Firmware Architecture

Firmware SHOULD be modular.

Expected logical modules include:

- System
- IMU driver
- Calibration
- Motion processing
- Filtering
- Controls
- Encoder
- HID
- Profiles
- Persistent storage
- Power management
- Diagnostics

Hardware-specific logic SHOULD remain separated from higher-level application logic.

---

# 21. Development Environment

Primary development environment:

- VS Code
- PlatformIO

The firmware must be buildable and flashable from the development environment without requiring Arduino IDE.

The project SHOULD support:

- Build
- Flash
- Serial monitoring
- Debug logging
- Automated testing where practical

---

# 22. AI-Assisted Development

The project will use multiple development agents/tools.

Potential roles:

### Project Management
Traycer

### Planning
GSD

### Primary Implementation
Claude Code

### Code Review / Secondary Engineering
Codex

### Iterative Development
Ralph loop

All agents MUST use the repository documentation as the source of truth.

Agents MUST NOT invent undocumented hardware behavior.

---

# 23. Documentation Requirements

The following documents SHOULD remain synchronized with the implementation:

- PRD
- Hardware specification
- Controls specification
- Architecture
- Firmware documentation
- Testing documentation
- Architecture decision records

Hardware changes MUST be reflected in the relevant documentation.

---

# 24. Testing Strategy

Testing will occur in stages.

## Stage 1: Hardware Communication

Verify:

- ESP32 startup
- I2C communication
- MPU6050 detection
- Sensor readings

## Stage 2: Sensor Processing

Verify:

- Gyroscope readings
- Accelerometer readings
- Calibration
- Filtering

## Stage 3: Cursor Engine

Verify:

- Motion-to-cursor mapping
- Sensitivity
- Dead zone
- Re-centering
- Smoothing

## Stage 4: HID

Verify:

- Bluetooth pairing
- Mouse movement
- Left click
- Right click
- Middle click
- Scrolling

## Stage 5: Advanced Controls

Verify:

- Profiles
- Custom mappings
- Macros
- Additional encoder functions

## Stage 6: Long-duration Testing

Verify:

- Stability
- Connection reliability
- Sensor drift
- Memory behavior
- Configuration persistence
- Power behavior

---

# 25. Prototype Strategy

Development will occur incrementally.

The first prototype MUST prioritize:

1. ESP32
2. MPU6050
3. Sensor communication
4. Gyroscope data
5. Basic cursor movement
6. Bluetooth HID
7. Basic mouse buttons
8. Scroll input

Advanced features will be added after the core system is stable.

---

# 26. Future Hardware Revision

After the prototype is validated, the project MAY progress to a custom PCB.

Potential V2 hardware:

- Custom PCB
- ESP32 module
- MPU6050 or improved IMU
- Integrated encoder
- Integrated buttons
- Battery
- Charging circuit
- Power management
- LEDs
- USB-C
- Custom enclosure

The PCB MUST NOT be finalized until the prototype validates the electrical design and physical control layout.

---

# 27. Enclosure

The final device SHOULD use a custom enclosure.

The enclosure should be designed around:

- Natural hand position
- Thumb access to scroll wheel
- Index-finger access to primary click
- Additional button accessibility
- IMU orientation
- PCB dimensions
- Battery placement
- USB access
- Power switch access

The enclosure design will begin only after the control layout and electronics are validated.

---

# 28. Non-Goals for Initial Version

The following are NOT required for the initial prototype:

- Custom PCB
- Final 3D-printed enclosure
- Desktop configuration application
- RGB lighting
- Battery optimization
- Advanced gaming features
- Wireless firmware updates
- Cloud configuration
- Multi-device synchronization

These may be considered later.

---

# 29. Open Questions

The following decisions remain open:

- Exact physical button layout
- Exact ANO encoder implementation
- Number of additional buttons
- Profile count
- Macro system
- Exact motion algorithm
- Sensor filtering algorithm
- Cursor acceleration model
- X/Y sensitivity model
- Recenter button
- LED configuration
- Battery type
- Charging circuit
- Power switch
- Final USB implementation
- Custom PCB architecture
- Final enclosure geometry
- Whether an improved IMU should replace MPU6050
- Configuration interface
- Firmware update mechanism

---

# 30. Requirements Classification

## MUST

Core functionality required for V1.

## SHOULD

Important functionality that should be implemented if practical.

## MAY

Optional functionality.

## FUTURE

Features intentionally deferred to a later revision.

## TBD

A decision that has not yet been finalized.

---

# 31. Current Project Status

### Hardware

- [x] ESP32 DevKit obtained
- [x] ESP32-WROOM-32 identified
- [ ] MPU6050 obtained
- [ ] Scroll encoder selected
- [ ] Buttons selected
- [ ] Power system selected
- [ ] Prototype wiring completed

### Software

- [ ] Repository structure
- [ ] PRD
- [ ] Hardware specification
- [ ] Control specification
- [ ] Architecture
- [ ] PlatformIO setup
- [ ] ESP32 test firmware
- [ ] MPU6050 driver
- [ ] Motion engine
- [ ] Bluetooth HID
- [ ] Button system
- [ ] Scroll system
- [ ] Configuration system
- [ ] Profile system

### Hardware Revision

- [ ] Prototype validated
- [ ] Schematic
- [ ] PCB
- [ ] PCB prototype
- [ ] Enclosure
- [ ] Final V1

---

# 32. Definition of Done

The Air Mouse V1 will be considered functionally complete when:

1. The ESP32 boots reliably.
2. The MPU6050 provides reliable motion data.
3. Calibration works.
4. Cursor movement is controllable and responsive.
5. The device connects through Bluetooth HID.
6. Basic mouse buttons work.
7. Scrolling works.
8. Re-centering works.
9. Configuration can be stored persistently.
10. The firmware can recover from invalid configuration.
11. Core functionality operates without dedicated desktop software.
12. The firmware is documented and modular.
13. Hardware behavior has been tested on the physical device.
14. Known limitations are documented.

---

# 33. Product Vision

The final device should feel less like a generic air mouse and more like a personally designed input instrument.

The user should be able to decide:

- How the cursor moves
- How sensitive it is
- What every button does
- How scrolling behaves
- How profiles work
- What shortcuts are available
- How the device responds to different gestures

The hardware and firmware should be designed so that these behaviors can evolve without requiring a complete redesign of the device.