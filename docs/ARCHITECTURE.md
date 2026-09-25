# Air Mouse Architecture Specification

**Project:** Personal ESP32 Air Mouse  
**Document:** System and Firmware Architecture  
**Version:** 0.1.0  
**Status:** Draft

---

# 1. Purpose

This document defines the software and system architecture of the Air Mouse.

The architecture is intended to keep hardware access, sensor processing, input handling, HID communication, configuration, and higher-level behavior separated into maintainable modules.

The architecture must support incremental development and allow individual subsystems to be tested independently.

---

# 2. Architectural Goals

The system SHOULD be:

- Modular
- Testable
- Low latency
- Hardware-aware
- Configurable
- Expandable
- Resistant to accidental coupling
- Suitable for AI-assisted development
- Suitable for future custom PCB hardware

The architecture should allow hardware details to change without requiring major changes to application-level behavior.

---

# 3. High-Level System

The overall system is:

```text
                    ┌─────────────────────┐
                    │      MPU6050        │
                    │ Gyro + Accelerometer│
                    └──────────┬──────────┘
                               │ I2C
                               ▼
                    ┌─────────────────────┐
                    │    IMU Driver       │
                    └──────────┬──────────┘
                               ▼
                    ┌─────────────────────┐
                    │    Calibration      │
                    └──────────┬──────────┘
                               ▼
                    ┌─────────────────────┐
                    │   Sensor Filter     │
                    └──────────┬──────────┘
                               ▼
                    ┌─────────────────────┐
                    │   Motion Engine     │
                    └──────────┬──────────┘
                               ▼
                    ┌─────────────────────┐
                    │   Cursor Engine     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    HID Manager      │
                    └──────────┬──────────┘
                               │ Bluetooth
                               ▼
                           Computer


    L / M / R Buttons ──┐
                        │
    Scroll Encoder ─────┼──► Input Manager ─► Action Mapper
                        │                         │
    Future Controls ────┘                         ▼
                                           HID / System Action
```

---

# 4. Major Software Layers

The firmware is divided into the following conceptual layers:

```text
Application / Behavior
        ↓
Action Mapping
        ↓
Input + Motion Processing
        ↓
HID
        ↓
Hardware Abstraction
        ↓
ESP32 Hardware
```

The exact implementation may differ, but logical separation should remain.

---

# 5. Firmware Modules

Expected firmware modules:

```text
firmware/src/

├── main.cpp
│
├── core/
│   ├── system
│   └── configuration
│
├── imu/
│   ├── mpu6050
│   └── calibration
│
├── motion/
│   ├── filter
│   └── motion_engine
│
├── controls/
│   ├── buttons
│   └── encoder
│
├── hid/
│   ├── mouse
│   └── keyboard
│
├── profiles/
│
├── storage/
│
├── power/
│
└── diagnostics/
```

Modules should communicate through well-defined interfaces.

---

# 6. Main Application

`main.cpp` should remain small.

Its responsibilities should primarily include:

1. System initialization.
2. Hardware/module initialization.
3. Starting the application runtime.
4. Coordinating high-level execution.

`main.cpp` should NOT become a dumping ground for sensor processing, button logic, HID implementation, or configuration logic.

---

# 7. System Initialization

Initialization should occur in a predictable sequence.

Conceptual sequence:

```text
Boot
 ↓
Basic hardware initialization
 ↓
Configuration storage initialization
 ↓
Load configuration
 ↓
GPIO initialization
 ↓
I2C initialization
 ↓
MPU6050 initialization
 ↓
Control initialization
 ↓
Bluetooth/HID initialization
 ↓
Calibration / startup state
 ↓
Ready
```

Initialization failures should be detectable.

The firmware should provide useful diagnostics when a required peripheral cannot be initialized.

---

# 8. IMU Driver

The IMU driver is responsible for communicating directly with the MPU6050.

Responsibilities:

- Device initialization
- Register configuration
- Sensor reads
- Sensor configuration
- Optional interrupt configuration
- Error detection
- Raw sensor data representation

The driver SHOULD expose sensor data without embedding cursor behavior.

Example conceptual interface:

```text
initialize()
configure()
read()
get_acceleration()
get_gyroscope()
```

The driver MUST NOT decide what sensor movement means for the mouse.

---

# 9. Calibration Layer

Calibration converts raw sensor measurements into usable measurements.

Responsibilities may include:

- Gyroscope bias estimation
- Accelerometer offset estimation
- Startup calibration
- Recalibration
- Reference orientation
- Re-centering

The calibration layer should be independent of HID behavior.

---

# 10. Filtering

Filtering reduces sensor noise and undesirable movement.

Potential filtering approaches include:

- Moving average
- Low-pass filtering
- Exponential smoothing
- Complementary filtering
- Other appropriate digital filters

The exact algorithm is TBD and must be selected based on measured sensor behavior.

Filtering MUST balance:

- Smoothness
- Responsiveness
- Computational cost
- Latency

The filter should not introduce excessive lag.

---

# 11. Motion Engine

The motion engine converts calibrated sensor information into logical device motion.

Responsibilities:

- Interpret sensor axes
- Apply orientation mapping
- Apply sensitivity
- Apply dead zone
- Apply acceleration
- Apply response curves
- Produce logical X/Y cursor deltas

Conceptual interface:

```text
MotionInput
    ↓
MotionEngine
    ↓
CursorDelta
```

The motion engine MUST NOT directly communicate with physical GPIO.

---

# 12. Cursor Engine

The cursor engine converts logical movement into HID mouse movement.

Responsibilities:

- X/Y movement generation
- Cursor scaling
- Clamping where required
- Report timing
- Motion enable/disable
- Integration with profile settings

The cursor engine should not know the electrical details of the MPU6050.

---

# 13. Input Manager

The input manager receives physical input from:

- L button
- M button
- R button
- Scroll encoder
- Future buttons
- Future directional controls

It converts electrical input into logical events.

Example:

```text
GPIO transition
      ↓
Button Driver
      ↓
Debounce
      ↓
Logical Input Event
```

---

# 14. Button Processing

Button processing should handle:

- Debouncing
- Press
- Release
- Hold
- Long press
- Optional double press

The exact interaction types will be enabled only when required by the product requirements.

Button drivers should report events rather than directly performing mouse actions.

Example:

```text
BUTTON_L_PRESSED
BUTTON_L_RELEASED
BUTTON_L_HELD
```

---

# 15. Encoder Processing

The scroll encoder layer is responsible for:

- Reading encoder state
- Determining direction
- Detecting steps
- Debouncing where required
- Reporting logical rotation events

Example:

```text
ENCODER_CW
ENCODER_CCW
```

If the selected encoder includes additional buttons, those buttons should appear as normal logical input events.

---

# 16. Action Mapper

The action mapper separates physical inputs from their assigned behavior.

Example:

```text
Physical input:
BUTTON_L

        ↓

Logical event:
BUTTON_L_PRESSED

        ↓

Profile mapping:

LEFT_CLICK

        ↓

HID Manager

        ↓

Computer
```

This is one of the most important abstraction boundaries in the system.

It allows button mappings to change without rewriting the button driver.

---

# 17. HID Manager

The HID manager handles communication with the host computer.

Expected HID capabilities:

- Mouse movement
- Left click
- Right click
- Middle click
- Scroll
- Optional keyboard input
- Optional media keys

The HID layer should not know how the input was generated.

For example, it should not care whether a left-click originated from:

- Physical button
- Macro
- Profile
- Future gesture

It should simply receive a request to generate the corresponding HID action.

---

# 18. Bluetooth Layer

The Bluetooth subsystem is responsible for:

- Initialization
- Pairing/connection state
- HID registration
- Connection monitoring
- Reconnection behavior where supported
- Disconnection handling

The exact Bluetooth stack/API will be selected during implementation based on the chosen ESP32 development framework.

---

# 19. Profiles

Profiles contain user-configurable behavior.

A profile may include:

```text
Cursor sensitivity
X sensitivity
Y sensitivity
Dead zone
Smoothing
Acceleration
Scroll sensitivity
Button mappings
Encoder mappings
Motion enable state
```

The profile manager should provide an interface such as:

```text
load_profile()
save_profile()
switch_profile()
get_active_profile()
```

---

# 20. Configuration Storage

Persistent configuration should be separated from the profile logic.

Responsibilities:

- Read stored configuration
- Write configuration
- Validate configuration
- Provide defaults
- Detect invalid/corrupt data
- Version stored configuration

The storage layer should hide the underlying ESP32 storage mechanism from higher-level modules.

---

# 21. Configuration Versioning

Stored configuration SHOULD include a version identifier.

Conceptually:

```text
Configuration
├── version
├── active_profile
├── profile data
└── system settings
```

If a future firmware version changes the configuration format, the firmware should be able to detect an incompatible version.

---

# 22. Re-centering

Re-centering is a motion-system operation.

The architecture should treat it as an event:

```text
RECENTER_REQUEST
        ↓
Motion Engine
        ↓
Capture current reference
        ↓
Reset relative motion
```

The input that triggers the request should remain separate from the actual recenter implementation.

This allows the recenter action to be assigned to different physical controls.

---

# 23. Power Management

Power management will initially be minimal because the prototype is USB powered.

Future responsibilities may include:

- Idle detection
- Sleep
- Wake
- Battery state
- Charging state
- Low-battery behavior

Power management should not interfere with real-time motion processing.

---

# 24. Diagnostics

A diagnostics subsystem should provide development-time visibility.

Potential diagnostics:

- Sensor initialization state
- I2C errors
- Bluetooth state
- Input events
- Sensor values
- Calibration state
- Active profile
- Battery state
- Firmware version

Debug output should be controllable so that production builds do not generate unnecessary serial traffic.

---

# 25. Runtime Model

The firmware should be designed around responsive event/data processing.

Conceptually:

```text
                 ┌───────────────┐
                 │ Sensor Update │
                 └───────┬───────┘
                         ↓
                 ┌───────────────┐
                 │ Motion Engine │
                 └───────┬───────┘
                         ↓
                 ┌───────────────┐
                 │  Cursor/HID   │
                 └───────────────┘


Buttons ───────► Input Manager ───► Action Mapper ───► HID

Encoder ───────► Input Manager ───► Action Mapper ───► HID
```

The implementation may use a loop, tasks, timers, interrupts, or a combination depending on the selected framework.

The architecture should prioritize responsiveness over unnecessary complexity.

---

# 26. Timing and Latency

The motion path is latency-sensitive.

The following should be avoided in the critical motion path:

- Long blocking delays
- Slow storage operations
- Excessive logging
- Unnecessary dynamic allocation
- Long-running computations

Sensor acquisition, processing, and HID reporting should operate at appropriate rates.

Exact timing values will be established during implementation and measurement.

---

# 27. Interrupt Strategy

Interrupts may be used for:

- Encoder state changes
- Button events
- MPU6050 data-ready signal
- Hardware timers

Interrupt handlers should:

- Be short
- Avoid blocking
- Avoid heavy computation
- Avoid complex storage operations
- Signal or queue work for normal execution where appropriate

---

# 28. Error Handling

The firmware should distinguish between:

### Recoverable errors

Examples:

- Temporary I2C read failure
- Temporary Bluetooth disconnection
- Invalid configuration

### Critical errors

Examples:

- Required sensor unavailable
- Hardware initialization failure
- Persistent configuration failure

The firmware should fail predictably rather than silently continuing with invalid hardware state.

---

# 29. Memory Strategy

The firmware should avoid unnecessary dynamic memory allocation.

Prefer:

- Static allocation where practical
- Fixed-size buffers
- Explicit ownership
- Small data structures

Memory usage should be considered when adding:

- Profiles
- Macros
- Logging
- HID functionality
- Future features

---

# 30. Module Dependency Rules

The following dependency direction is preferred:

```text
Application
    ↓
Action Mapping
    ↓
Input / Motion
    ↓
HID / Services
    ↓
Drivers
    ↓
Hardware
```

Lower-level drivers MUST NOT depend on high-level application behavior.

For example:

```text
GOOD:

MPU6050 Driver
      ↓
Motion Engine


BAD:

MPU6050 Driver
      ↓
"Move mouse cursor"
```

The driver reports sensor data.

The higher-level system decides what that data means.

---

# 31. Hardware Abstraction

Hardware-specific details should be isolated.

Examples:

- GPIO numbers
- I2C pins
- Sensor register configuration
- Button electrical polarity
- Encoder wiring

These details should not be duplicated across multiple modules.

A hardware configuration layer should provide the authoritative values.

---

# 32. Testing Architecture

The system should support multiple levels of testing.

## Unit Tests

Suitable for:

- Filters
- Motion calculations
- Dead zones
- Sensitivity curves
- Configuration validation
- Button state machines
- Encoder logic

## Integration Tests

Suitable for:

- IMU → motion engine
- Input → action mapper
- Action mapper → HID
- Profile → action mapping

## Hardware Tests

Required for:

- I2C
- MPU6050
- GPIO
- Encoder
- Bluetooth HID
- Power behavior

---

# 33. Future Extensibility

The architecture should allow future features without redesigning the entire firmware.

Potential future additions:

- Better IMU
- Additional sensors
- Gesture recognition
- More buttons
- Advanced macros
- Application-specific profiles
- Wireless firmware update
- Battery monitoring
- Configuration interface
- Custom PCB

New features should integrate through existing abstraction boundaries where possible.

---

# 34. AI Development Rules

All coding agents should:

1. Read `PRD.md` before implementing product behavior.
2. Read `HARDWARE.md` before changing hardware-related code.
3. Read `CONTROLS.md` before changing input behavior.
4. Follow this architecture document.
5. Avoid bypassing abstraction layers without justification.
6. Avoid changing public interfaces unnecessarily.
7. Keep modules small and testable.
8. Update documentation when architectural behavior changes.
9. Never invent hardware specifications.
10. Build and test before declaring implementation complete.

---

# 35. Current Architecture Status

- [x] Product concept defined
- [x] Initial control concept defined
- [x] ESP32 platform identified
- [x] MPU6050 selected as initial IMU candidate
- [ ] Exact hardware pin mapping
- [ ] Exact encoder interface
- [ ] Bluetooth HID stack selected
- [ ] Firmware framework selected
- [ ] Runtime/task model finalized
- [ ] Motion algorithm finalized
- [ ] Filtering algorithm finalized
- [ ] Configuration format finalized
- [ ] Profile format finalized

---

# 36. Architecture Definition of Done

The architecture will be considered sufficiently defined for implementation when:

1. Hardware interfaces are documented.
2. Firmware modules have clear responsibilities.
3. Module dependencies are understood.
4. Motion data flow is defined.
5. Input data flow is defined.
6. HID data flow is defined.
7. Configuration flow is defined.
8. Error handling strategy is defined.
9. Testing boundaries are defined.
10. AI agents can determine where a new feature belongs without guessing.

---

# 37. Guiding Principle

The firmware should follow one central architectural rule:

> **Hardware produces data. Processing gives that data meaning. Actions determine behavior. HID communicates the result.**

This separation allows the physical device, motion algorithm, control mappings, and user experience to evolve independently.
