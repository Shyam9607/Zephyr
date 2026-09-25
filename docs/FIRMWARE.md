# Air Mouse Firmware Specification

**Project:** Personal ESP32 Air Mouse  
**Document:** Firmware Specification  
**Version:** 0.1.0  
**Status:** Draft

---

# 1. Purpose

This document defines the firmware-level requirements, conventions, runtime behavior, build environment, module boundaries, configuration model, diagnostics, and implementation constraints for the Air Mouse.

The firmware is responsible for converting physical sensor/control inputs into Bluetooth HID behavior while keeping the system modular, testable, configurable, and maintainable.

---

# 2. Firmware Platform

## 2.1 Target Hardware

Primary target:

- ESP32-WROOM-32 based DevKit-style development board

The firmware MUST remain adaptable to the finalized GPIO and board configuration.

Hardware-specific values MUST NOT be scattered throughout the codebase.

---

# 3. Development Environment

Primary IDE/editor:

- Visual Studio Code

Primary build environment:

- PlatformIO

Arduino IDE is NOT required.

The firmware project should support:

- Build
- Upload/flash
- Serial monitor
- Automated tests where practical
- Debug logging
- Clean rebuild

---

# 4. Framework

The exact PlatformIO framework is TBD.

Candidates include:

- Arduino framework
- ESP-IDF

The framework MUST be selected after evaluating:

- Bluetooth HID support
- ESP32-WROOM-32 compatibility
- Library support
- Stability
- Build complexity
- Debugging support
- Long-term maintainability

The framework decision should be recorded as an Architecture Decision Record.

---

# 5. Firmware Directory

The intended firmware structure is:

```text
firmware/
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   ├── imu/
│   ├── motion/
│   ├── controls/
│   ├── hid/
│   ├── profiles/
│   ├── storage/
│   ├── power/
│   └── diagnostics/
│
├── include/
├── lib/
├── test/
└── platformio.ini
```

The exact module structure may evolve as implementation details become clearer.

---

# 6. Module Responsibilities

## Core

Responsible for:

- System initialization
- Runtime coordination
- Global system state
- Hardware configuration
- Firmware version information

Core MUST NOT contain detailed driver logic.

---

## IMU

Responsible for:

- MPU6050 communication
- Sensor configuration
- Raw sensor acquisition
- Sensor status

The IMU module MUST NOT directly generate mouse events.

---

## Motion

Responsible for:

- Calibration
- Filtering
- Sensor axis mapping
- Motion estimation
- Dead zone
- Sensitivity
- Acceleration
- Response curves
- Re-centering

---

## Controls

Responsible for:

- Button input
- Debouncing
- Encoder input
- Encoder direction
- Input event generation

Controls MUST produce logical events rather than directly executing application actions.

---

## HID

Responsible for:

- Mouse reports
- Keyboard reports where required
- Media reports where supported
- Bluetooth HID state
- Host connection state

---

## Profiles

Responsible for:

- Active profile
- Profile switching
- Profile data
- Profile validation

---

## Storage

Responsible for:

- Persistent configuration
- Configuration validation
- Defaults
- Versioning
- Save/load operations

---

## Power

Responsible for:

- Power state
- Future battery monitoring
- Future sleep/wake behavior
- Power-related state transitions

The initial prototype may contain only a minimal implementation.

---

## Diagnostics

Responsible for:

- Debug logging
- Runtime diagnostics
- Sensor status
- Connection status
- Error reporting

Diagnostics MUST be controllable to prevent excessive output during normal operation.

---

# 7. Main Program

`main.cpp` should remain intentionally small.

Conceptual structure:

```cpp
void setup()
{
    initialize_system();
    initialize_configuration();
    initialize_hardware();
    initialize_imu();
    initialize_controls();
    initialize_hid();
}

void loop()
{
    process_system();
}
```

The actual implementation may differ depending on the selected framework and runtime architecture.

Business logic SHOULD NOT accumulate inside `main.cpp`.

---

# 8. Initialization Requirements

Initialization should follow a predictable sequence:

```text
Boot
 ↓
System initialization
 ↓
Configuration initialization
 ↓
Hardware configuration
 ↓
I2C initialization
 ↓
MPU6050 initialization
 ↓
Control initialization
 ↓
Bluetooth/HID initialization
 ↓
Calibration/startup state
 ↓
Ready
```

Every required initialization step MUST have an observable success/failure state.

---

# 9. Runtime Requirements

The runtime must remain responsive to:

- IMU updates
- Button events
- Encoder events
- Bluetooth events
- Re-centering
- Profile changes

Long blocking operations SHOULD NOT occur in the main input/motion path.

---

# 10. Sensor Sampling

The MPU6050 should be sampled at a rate appropriate for responsive cursor control.

The exact sampling rate is TBD and must be determined through measurement.

The firmware SHOULD avoid:

- Unnecessarily slow polling
- Excessively high sampling rates without benefit
- Blocking sensor reads
- Excessive serial logging during sampling

---

# 11. Motion Pipeline

The firmware motion pipeline should follow:

```text
MPU6050
   ↓
Raw Sensor Data
   ↓
Calibration
   ↓
Axis Mapping
   ↓
Filtering
   ↓
Dead Zone
   ↓
Sensitivity
   ↓
Acceleration / Response Curve
   ↓
Cursor Delta
   ↓
HID Mouse Report
```

Each stage should remain independently testable where practical.

---

# 12. Motion Configuration

The firmware SHOULD support configuration for:

- Overall sensitivity
- X-axis sensitivity
- Y-axis sensitivity
- Dead zone
- Smoothing
- Acceleration
- Response curve
- Axis inversion
- Motion enable/disable

The configuration must not require recompiling the firmware for normal user-level changes where on-device configuration is supported.

---

# 13. Re-centering

The motion subsystem MUST support re-centering.

A recenter request should:

```text
Current sensor state
        ↓
Capture reference
        ↓
Reset relative motion
        ↓
Continue tracking
```

Re-centering must not require a reboot.

The trigger source is handled by the input/action system.

---

# 14. Input Processing

Physical inputs should pass through:

```text
Electrical Input
      ↓
Driver
      ↓
Debounce / State Machine
      ↓
Logical Event
      ↓
Action Mapper
      ↓
HID / System Action
```

Examples of logical events:

```text
BUTTON_L_PRESSED
BUTTON_L_RELEASED
BUTTON_M_PRESSED
BUTTON_R_PRESSED
ENCODER_CW
ENCODER_CCW
RECENTER_REQUEST
PROFILE_NEXT
```

---

# 15. Button Debouncing

Mechanical buttons MUST be debounced.

Debouncing may be implemented through:

- Time-based state machines
- Sampling
- Interrupt + deferred processing
- Other validated approaches

The chosen method must avoid:

- False presses
- Duplicate events
- Excessive latency

---

# 16. Encoder Processing

The scroll encoder should produce logical rotation events.

Example:

```text
Clockwise
    ↓
ENCODER_CW

Counter-clockwise
    ↓
ENCODER_CCW
```

The encoder implementation must correctly handle:

- Direction
- Step detection
- Contact bounce
- Rapid rotation

The exact decoder implementation is TBD.

---

# 17. Action Mapping

Action mapping is the layer between physical controls and behavior.

Example:

```text
BUTTON_L
   ↓
Action Mapper
   ↓
LEFT_CLICK
   ↓
HID Manager
```

The mapping SHOULD be profile-dependent.

This enables:

```text
Profile A:
L → Left Click

Profile B:
L → Play/Pause

Profile C:
L → Keyboard shortcut
```

without changing the physical button driver.

---

# 18. Interaction Types

The firmware MAY support:

- Single press
- Release
- Hold
- Long press
- Double press

The timing thresholds should be configurable only if there is a clear user benefit.

Complex interaction logic must not interfere with normal mouse responsiveness.

---

# 19. HID Requirements

The HID subsystem SHOULD support:

### Mouse

- Relative X movement
- Relative Y movement
- Left button
- Right button
- Middle button
- Vertical scroll
- Optional horizontal scroll

### Keyboard

Optional future support for:

- Individual keys
- Modifier keys
- Key combinations

### Media

Optional future support for:

- Play/pause
- Previous
- Next
- Volume
- Mute

The initial implementation should prioritize mouse HID functionality.

---

# 20. Bluetooth State

The firmware should maintain a logical Bluetooth state such as:

```text
DISCONNECTED
CONNECTING
CONNECTED
ERROR
```

The exact states depend on the selected Bluetooth framework/API.

Connection state may be exposed through diagnostics and optional LEDs.

---

# 21. Profiles

A profile should contain only configuration data.

Example:

```text
Profile
├── cursor_sensitivity
├── x_sensitivity
├── y_sensitivity
├── dead_zone
├── smoothing
├── acceleration
├── scroll_sensitivity
├── button_mappings
└── encoder_mappings
```

Profiles MUST NOT contain executable code.

---

# 22. Persistent Storage

Persistent settings should be stored using an appropriate ESP32 non-volatile storage mechanism.

The storage layer MUST provide:

- Read
- Write
- Validation
- Defaults
- Versioning

Invalid configuration should result in a safe default configuration rather than undefined behavior.

---

# 23. Configuration Version

Stored configuration should include a schema/version field.

Example:

```text
config_version = 1
```

If the configuration schema changes, the firmware must be able to detect incompatible stored data.

Migration may be implemented in a future version.

---

# 24. Logging

Logging should have levels.

Suggested levels:

```text
ERROR
WARN
INFO
DEBUG
TRACE
```

Normal production operation SHOULD use minimal logging.

High-frequency sensor data MUST NOT be logged continuously unless explicitly enabled for diagnostics.

---

# 25. Error Handling

Errors should be represented explicitly.

Examples:

```text
IMU_NOT_FOUND
I2C_ERROR
CONFIG_INVALID
BLUETOOTH_ERROR
HID_ERROR
```

The firmware should attempt recovery for recoverable errors when practical.

Critical errors should enter a predictable safe state.

---

# 26. Watchdog

The firmware SHOULD use appropriate watchdog functionality where supported.

The watchdog must not be used to mask software defects.

Any watchdog reset should be diagnosable during development.

---

# 27. Timing

Timing-sensitive operations include:

- IMU sampling
- Motion processing
- Button debouncing
- Encoder decoding
- HID reporting
- Bluetooth state handling

Timing values must be documented when finalized.

Magic timing constants SHOULD NOT be scattered throughout the code.

---

# 28. Memory Management

Firmware SHOULD prefer:

- Static allocation
- Fixed-size buffers
- Explicit ownership
- Deterministic memory usage

Avoid unnecessary:

- Dynamic allocation in high-frequency paths
- Large temporary objects
- Unbounded containers

Memory usage should be reviewed when adding major features.

---

# 29. Code Style

The project should use consistent formatting.

General rules:

- Clear names
- Small functions
- Single responsibility
- Minimal global state
- Explicit interfaces
- Avoid unnecessary abstraction
- Avoid duplicated hardware constants
- Comments should explain why, not merely what

Hardware constants should be centralized.

---

# 30. Configuration Constants

Configuration values should be categorized.

### Hardware constants

Examples:

```text
I2C pins
GPIO assignments
I2C address
interrupt pins
```

### Firmware defaults

Examples:

```text
Default sensitivity
Default scroll speed
Default profile
```

### Runtime configuration

Examples:

```text
Current sensitivity
Current profile
Button mapping
```

These categories should not be mixed.

---

# 31. Testing

The firmware SHOULD support:

## Unit testing

- Motion math
- Filters
- Dead zones
- Sensitivity
- Configuration validation
- Button state machines
- Encoder decoding

## Integration testing

- IMU + motion pipeline
- Controls + action mapper
- Profiles + action mapper
- Action mapper + HID

## Hardware testing

- I2C communication
- Sensor behavior
- GPIO behavior
- Bluetooth pairing
- HID operation
- Long-duration stability

---

# 32. Build Requirements

Before a feature is considered complete:

1. Firmware must compile successfully.
2. Relevant tests must pass.
3. No new compiler warnings should be introduced without justification.
4. Hardware-dependent behavior must be identified.
5. Documentation must be updated when behavior changes.

---

# 33. Flashing

The prototype should be flashable through USB using PlatformIO.

The workflow should support:

```text
Build
 ↓
Upload
 ↓
Monitor
```

The exact upload command and board configuration will be defined in `platformio.ini`.

---

# 34. Serial Diagnostics

The USB serial connection should be available during development for:

- Boot messages
- Sensor diagnostics
- Calibration output
- Bluetooth state
- Error messages
- Debugging

Serial output must not become a dependency for normal device operation.

---

# 35. Firmware Versioning

The firmware should expose a version identifier.

Example:

```text
AIR-MOUSE FW
Version: 0.1.0
```

The version should be accessible through diagnostics.

Firmware version and configuration schema version should be treated as separate concepts.

---

# 36. Feature Flags

Feature flags MAY be used during development for experimental functionality.

Examples:

```text
ENABLE_DEBUG_LOGGING
ENABLE_ADVANCED_FILTER
ENABLE_MACROS
ENABLE_MEDIA_KEYS
```

Feature flags should not become a substitute for proper architecture.

Experimental features should be removed or formalized before stable releases.

---

# 37. Security and Robustness

The firmware should:

- Validate stored configuration.
- Avoid unsafe buffer operations.
- Avoid unchecked input lengths.
- Handle unexpected Bluetooth state changes.
- Handle missing sensors gracefully.
- Avoid uncontrolled memory growth.

No network/cloud dependency is required for the core device.

---

# 38. Firmware Development Phases

## Phase 1

ESP32 development environment

```text
VS Code
PlatformIO
Build
Flash
Serial
```

## Phase 2

Basic hardware

```text
GPIO
I2C
MPU6050 detection
Raw sensor data
```

## Phase 3

Motion

```text
Calibration
Filtering
Motion engine
Cursor mapping
```

## Phase 4

Bluetooth HID

```text
Pairing
Mouse movement
Clicks
Scroll
```

## Phase 5

Advanced controls

```text
Button mapping
Encoder mapping
Recenter
Profiles
Persistent configuration
```

## Phase 6

Optimization

```text
Latency
Power
Memory
Reliability
Long-duration testing
```

---

# 39. Definition of Done

Firmware V1 is considered functionally complete when:

1. The ESP32 builds reliably.
2. Firmware can be flashed from VS Code/PlatformIO.
3. MPU6050 communication works.
4. Calibration works.
5. Motion processing works.
6. Cursor movement works.
7. Bluetooth HID connection works.
8. L/M/R buttons work.
9. Scroll works.
10. Re-centering works.
11. Configuration can be persisted.
12. The device recovers safely from invalid configuration.
13. Core behavior works without dedicated desktop software.
14. Major modules have tests where practical.
15. Documentation reflects the final implementation.
16. Physical hardware testing has been completed for core functions.

---

# 40. Firmware Principles

The firmware should follow these principles:

1. Keep the real-time path responsive.
2. Keep hardware access isolated.
3. Keep input events separate from actions.
4. Keep profiles separate from drivers.
5. Keep configuration separate from storage implementation.
6. Keep diagnostics controllable.
7. Prefer deterministic behavior.
8. Measure before optimizing.
9. Test incrementally.
10. Do not add complexity without a clear requirement.

---

# 41. Guiding Principle

The firmware should be treated as a small embedded system, not as a single Arduino sketch.

Each subsystem should have one clear responsibility, a defined interface, and a testable boundary.

The final firmware should be understandable by both humans and development agents without requiring undocumented assumptions.
