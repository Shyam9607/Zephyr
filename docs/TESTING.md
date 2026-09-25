# Air Mouse Testing Specification

**Project:** Personal ESP32 Air Mouse  
**Document:** Testing Specification  
**Version:** 0.1.0  
**Status:** Draft

---

# 1. Purpose

This document defines the testing strategy for the Air Mouse.

Testing is divided into software, firmware integration, hardware, communication, ergonomics, performance, reliability, and final acceptance testing.

The goal is to catch problems as early as possible and prevent hardware-specific issues from being hidden behind higher-level software behavior.

---

# 2. Testing Principles

The project follows these principles:

1. Test incrementally.
2. Test one subsystem before combining it with another.
3. Prefer automated tests for deterministic logic.
4. Use physical hardware tests for hardware-dependent behavior.
5. Reproduce bugs before attempting complex fixes.
6. Record important test results.
7. Do not declare a feature complete based only on successful compilation.
8. Test both normal and failure conditions.
9. Keep hardware tests repeatable where practical.
10. Validate the actual user experience before finalizing the design.

---

# 3. Testing Levels

The project uses five primary levels:

```text
Unit Tests
    ↓
Module Tests
    ↓
Integration Tests
    ↓
Hardware Tests
    ↓
System / User Tests
```

---

# 4. Unit Testing

Unit tests should cover logic that can be tested without physical hardware.

Potential targets:

- Motion calculations
- Dead-zone calculations
- Sensitivity scaling
- Acceleration curves
- Response curves
- Filter calculations
- Configuration validation
- Profile switching
- Button state machines
- Debounce logic
- Encoder decoding
- Action mapping

---

# 5. Motion Algorithm Tests

The motion engine should be tested using known input values.

Example:

```text
Input gyro value
        ↓
Motion processing
        ↓
Expected cursor delta
```

Tests should verify:

- Zero input produces zero movement.
- Positive input produces expected direction.
- Negative input produces expected opposite direction.
- Sensitivity changes scale movement correctly.
- Dead zone suppresses sufficiently small inputs.
- Axis inversion behaves correctly.
- Acceleration behaves predictably.
- Output remains within expected limits.

---

# 6. Filter Tests

Filtering must be tested for both stability and responsiveness.

Tests should verify:

- Noisy input is reduced.
- Stable input remains stable.
- Sudden movement is still detected.
- Filter does not introduce unacceptable latency.
- Filter handles extreme values safely.
- Filter state initializes correctly.

Different filtering approaches may be compared experimentally.

---

# 7. Button Tests

Button logic must be tested for:

- Press
- Release
- Debouncing
- Long press
- Hold
- Double press if implemented
- Rapid repeated presses
- Simultaneous button presses where supported

Example:

```text
Physical press
    ↓
Debounce
    ↓
One logical PRESS event
```

A single physical press MUST NOT produce multiple logical presses because of contact bounce.

---

# 8. Encoder Tests

The encoder subsystem must verify:

- Clockwise rotation
- Counter-clockwise rotation
- Single step
- Multiple rapid steps
- Direction correctness
- Bounce handling
- Missing/invalid transitions where relevant

Expected mapping must be explicitly defined.

Example:

```text
CW  → Scroll Down
CCW → Scroll Up
```

The mapping may later be configurable.

---

# 9. Action Mapping Tests

The action mapper must verify that physical inputs are correctly converted into logical actions.

Example:

```text
BUTTON_L
    ↓
LEFT_CLICK
```

Tests should verify:

- Default mappings
- Profile-specific mappings
- Remapped buttons
- Disabled inputs
- Invalid mappings
- Profile switching

---

# 10. Configuration Tests

Configuration handling must test:

- Default configuration
- Valid configuration
- Invalid configuration
- Missing configuration
- Corrupt configuration
- Configuration version mismatch
- Save
- Load
- Profile persistence

The firmware must fall back to safe defaults when configuration is invalid.

---

# 11. Integration Testing

Integration tests verify that multiple modules work correctly together.

Important integration paths include:

```text
MPU6050
  ↓
IMU Driver
  ↓
Calibration
  ↓
Filter
  ↓
Motion Engine
  ↓
Cursor Engine
  ↓
HID
```

and:

```text
Button
  ↓
Input Manager
  ↓
Action Mapper
  ↓
HID
```

and:

```text
Encoder
  ↓
Input Manager
  ↓
Action Mapper
  ↓
HID
```

---

# 12. ESP32 Hardware Tests

The physical ESP32 must be tested before full firmware development.

Initial checks:

- Board powers correctly.
- USB connection works.
- Firmware can be flashed.
- Serial monitor works.
- Reset works.
- Bootloader mode works.
- No unexpected overheating occurs.

---

# 13. I2C Tests

Before implementing the complete motion system:

1. Initialize I2C.
2. Detect the MPU6050.
3. Verify the expected device identity where applicable.
4. Read known registers.
5. Read accelerometer data.
6. Read gyroscope data.
7. Confirm stable communication.

Failure conditions should be logged.

---

# 14. MPU6050 Tests

The sensor must be tested physically.

Verify:

- Accelerometer readings change when the device is tilted.
- Gyroscope readings change when the device rotates.
- Axes correspond to the documented physical orientation.
- Sensor readings stabilize when stationary.
- Calibration reduces measurable bias.
- No unexpected axis inversion occurs.

The physical sensor orientation must be recorded in `HARDWARE.md`.

---

# 15. Bluetooth HID Tests

The Bluetooth subsystem must verify:

- Device discovery
- Pairing
- Connection
- Reconnection where supported
- Mouse identification
- Cursor movement
- Left click
- Middle click
- Right click
- Vertical scrolling
- Disconnection handling

Testing should be performed on the intended host computer.

---

# 16. Cursor Tests

Cursor behavior must be tested using real hardware.

Verify:

- Small hand movements produce controlled cursor movement.
- Large movements remain controllable.
- Cursor direction is intuitive.
- Re-centering works.
- Cursor does not drift excessively while stationary.
- Sensitivity changes behave predictably.
- Dead zone behaves correctly.
- Filtering does not make movement feel sluggish.

---

# 17. Re-centering Tests

Test sequence:

1. Connect the device.
2. Hold it in a chosen neutral position.
3. Trigger recenter.
4. Move the device.
5. Verify movement occurs relative to the new reference.
6. Repeat from a different orientation.

Verify that recentering does not require rebooting.

---

# 18. Scroll Tests

Verify:

- Clockwise rotation produces the configured direction.
- Counter-clockwise rotation produces the opposite direction.
- Slow scrolling works.
- Fast scrolling works.
- Rapid direction changes work.
- Scroll sensitivity behaves correctly.
- Encoder bounce does not produce unwanted scroll events.

---

# 19. Profile Tests

For each profile:

1. Load the profile.
2. Verify settings.
3. Test button mappings.
4. Test motion settings.
5. Test scroll settings.
6. Switch profiles.
7. Verify behavior changes.
8. Restart the ESP32.
9. Verify persistent profile state if persistence is enabled.

---

# 20. Persistent Storage Tests

Test:

```text
Save
 ↓
Power off/reset
 ↓
Boot
 ↓
Load
 ↓
Verify
```

Also test:

- Invalid stored data
- Version mismatch
- Partial/corrupt data
- Reset to defaults

The device must recover safely from invalid stored configuration.

---

# 21. Performance Testing

Important performance measurements include:

- Sensor sampling rate
- Motion processing time
- HID report rate
- Input-to-cursor latency
- Bluetooth connection latency
- Memory usage
- CPU usage where measurable

Exact target values are TBD and should be established after the first functional prototype.

---

# 22. Latency Testing

The goal is to make cursor movement feel immediate and predictable.

Potential measurement approach:

```text
Physical movement
      ↓
Sensor detection
      ↓
Processing
      ↓
HID transmission
      ↓
Host cursor response
```

The project should measure this path where practical rather than relying only on subjective impressions.

---

# 23. Stability Testing

The device should undergo extended operation.

Suggested tests:

- 30-minute continuous operation
- 1-hour continuous operation
- Repeated Bluetooth reconnects
- Repeated sensor reads
- Repeated button presses
- Repeated encoder use
- Repeated profile switching

Monitor for:

- Crashes
- Resets
- Memory growth
- Bluetooth failures
- Sensor lockups
- Configuration corruption
- Excessive heat

Longer tests may be added once the prototype is stable.

---

# 24. Failure and Recovery Testing

The firmware should be tested against abnormal conditions.

Examples:

- MPU6050 disconnected during startup
- I2C communication failure
- Bluetooth disconnect
- Invalid configuration
- Unexpected button state
- Encoder noise
- Rapid input
- Repeated reset
- Power interruption

The firmware should fail predictably and recover where practical.

---

# 25. Power Testing

USB-powered prototype:

- Stable operation over USB
- No unexpected resets
- Normal USB connection behavior

Future battery version:

- Startup
- Normal operation
- Idle
- Sleep
- Wake
- Low battery
- Charging
- Power-off
- Battery protection

Battery testing must follow the electrical limits of the final power architecture.

---

# 26. Thermal Testing

During extended operation verify:

- ESP32 temperature remains reasonable.
- Regulators do not overheat.
- Charging circuitry remains within expected limits in the battery version.
- No component becomes excessively hot.

Unexpected heating must be investigated before continued operation.

---

# 27. Ergonomic Testing

The physical device should be tested by the intended user.

Evaluate:

- Grip comfort
- Button reach
- Thumb position
- Scroll accessibility
- Accidental presses
- Wrist comfort
- Cursor control
- Re-centering convenience
- Device weight
- Balance

The physical design should be changed based on observed problems before the custom PCB/enclosure is finalized.

---

# 28. Control Layout Testing

Test the proposed layout:

```text
             L   M   R


                ◯
              SCROLL
```

Evaluate whether:

- L/M/R are easily distinguishable.
- Buttons can be pressed without looking.
- Scroll wheel is naturally reachable by the thumb.
- Scrolling does not destabilize the grip.
- The user can maintain control while clicking.
- The layout remains comfortable during long sessions.

---

# 29. Host Compatibility

Initial target:

- Windows PC

Future testing may include:

- Linux
- macOS
- Android
- Other Bluetooth HID hosts

Core HID functionality should follow standard behavior where practical.

---

# 30. Regression Testing

Whenever firmware changes are made, previously working functionality should be rechecked.

Minimum regression set:

- Build
- Flash
- Boot
- MPU6050 detection
- Cursor movement
- L/M/R
- Scroll
- Bluetooth connection
- Recenter
- Configuration persistence

Major architectural changes require broader regression testing.

---

# 31. Test Documentation

Important test results should be recorded.

Suggested format:

```text
Test ID:
Date:
Firmware version:
Hardware revision:
Test:
Expected:
Observed:
Result:
Notes:
```

Failures should include enough information to reproduce the problem.

---

# 32. Test IDs

Tests SHOULD use stable identifiers.

Example:

```text
HW-001  ESP32 boot
HW-002  USB flashing
HW-003  I2C detection
HW-004  MPU6050 read
HW-005  MPU6050 orientation

FW-001  Motion zero input
FW-002  Motion sensitivity
FW-003  Dead zone
FW-004  Button debounce
FW-005  Encoder direction

HID-001 Bluetooth pairing
HID-002 Mouse movement
HID-003 Left click
HID-004 Middle click
HID-005 Right click
HID-006 Scroll

SYS-001 Recenter
SYS-002 Profile switching
SYS-003 Configuration persistence
SYS-004 Long-duration operation
```

The numbering can expand as the project grows.

---

# 33. Definition of Done for Features

A feature is not considered complete merely because the code compiles.

A feature should satisfy:

1. Requirements are documented.
2. Implementation exists.
3. Code builds successfully.
4. Relevant automated tests pass.
5. Relevant hardware tests pass.
6. Failure behavior is understood.
7. Documentation is updated.
8. No known regression is introduced.

---

# 34. Bug Handling

When a bug is discovered:

```text
Reproduce
   ↓
Document
   ↓
Identify affected subsystem
   ↓
Create minimal fix
   ↓
Test fix
   ↓
Run regression tests
   ↓
Document result
```

Agents should not perform broad unrelated refactors while fixing a specific hardware or firmware bug unless explicitly requested.

---

# 35. AI Agent Testing Rules

AI development agents MUST:

- Build before declaring implementation complete.
- Run relevant automated tests.
- Identify hardware-dependent tests.
- Never claim a physical test was performed if it was not.
- Clearly distinguish simulated/unit results from real hardware results.
- Report failures instead of hiding them.
- Avoid changing tests merely to make failing code pass.
- Preserve regression coverage.
- Update test documentation when behavior changes.

---

# 36. Current Testing Status

## Software

- [ ] Test framework configured
- [ ] Unit test structure
- [ ] Motion tests
- [ ] Filter tests
- [ ] Button tests
- [ ] Encoder tests
- [ ] Configuration tests

## Hardware

- [ ] ESP32 boot test
- [ ] USB flashing
- [ ] I2C test
- [ ] MPU6050 test
- [ ] Button test
- [ ] Encoder test

## System

- [ ] Bluetooth pairing
- [ ] HID mouse
- [ ] Cursor movement
- [ ] Scroll
- [ ] Recenter
- [ ] Profiles
- [ ] Persistent configuration
- [ ] Long-duration stability

---

# 37. Testing Definition of Done

The testing system is considered sufficiently established when:

1. Unit testing infrastructure exists.
2. Hardware test procedures are documented.
3. Core sensor tests are defined.
4. Core input tests are defined.
5. HID tests are defined.
6. Regression tests are defined.
7. Failure scenarios are defined.
8. Test results can be recorded consistently.
9. AI agents can determine what must be tested before declaring a task complete.

---

# 38. Guiding Principle

Testing must answer two different questions:

> **"Does the code work?"**

and

> **"Does the physical device actually behave the way the user expects?"**

Both are required for the Air Mouse to be considered functional.
