# SPEC.md — Project Specification

> **Status**: `FINALIZED`
>
> 🔒 **Planning Lock Active**: Specification is finalized. Scope changes require explicit review.

---

## 1. Vision

Zephyr is a modular, ESP32-based wireless air mouse and handheld pointing controller designed to operate as a standard Bluetooth HID peripheral. It translates hand and wrist motion detected by an MPU6050 inertial measurement unit (IMU) into computer cursor movement, complemented by physical Left, Middle, and Right mouse buttons, a thumb-accessible scroll control, recentering capability, configurable input-to-action mapping, and persistent on-device configuration and profiles. The system is built prototype-first and documentation-first, requiring no proprietary desktop companion application for standard operation.

---

## 2. Goals

1. **Modular Firmware Architecture** — Establish a clean, decoupled firmware structure separating hardware drivers, sensor acquisition, motion filtering, cursor scaling, input management, action mapping, HID reporting, configuration storage, and diagnostics per [ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md) and [docs/FIRMWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md).
2. **Motion Acquisition & Calibration** — Interface with the MPU6050 over I2C to capture 3-axis gyroscope and accelerometer data, providing a software calibration routine to correct zero-rate bias and drift ([ADR-0003](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0003-imu.md), [docs/HARDWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/HARDWARE.md)).
3. **Motion Processing & Cursor Engine** — Convert raw angular velocity into smooth, responsive, low-latency cursor movement with configurable deadzone, noise filtering, sensitivity scaling, and acceleration curves ([docs/ARCHITECTURE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/ARCHITECTURE.md), [docs/PRD.md §5](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md#L125-L150)).
4. **Wireless Bluetooth HID Interface** — Function as a standard Bluetooth Low Energy HID mouse recognized natively by host operating systems without custom drivers ([ADR-0004](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0004-bluetooth-hid.md)).
5. **Physical Mouse Controls** — Support physical inputs for Left Click, Middle Click, and Right Click, with debouncing and configurable event generation ([docs/CONTROLS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md)).
6. **Dedicated Scroll Mechanism** — Support rotary/directional scrolling (candidate: Adafruit ANO navigation wheel per [ADR-0007](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0007-scroll-control.md)), mapped as abstract input events to standard HID vertical wheel reports.
7. **Cursor Recentering** — Provide a logical recentering function to re-anchor the cursor reference orientation and prevent posture drift.
8. **On-Device Persistent Configuration** — Store versioned configuration schemas, motion profiles, and input mappings on ESP32 non-volatile storage (NVS), featuring automatic detection and safe fallback to defaults if data is corrupt or incompatible ([ADR-0008](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0008-on-device-configuration.md)).
9. **Incremental Testability** — Enable host-based unit testing for algorithmic components (filtering, scaling, config serialization, action mapping) ahead of physical hardware bring-up ([docs/TESTING.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md)).

---

## 3. Non-Goals (Out of Scope for Prototype V1)

- **Custom PCB**: No custom printed circuit board design, schematics finalization, or fabrication; development remains breadboard/DevKit prototype-first ([ADR-0005](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0005-prototype-first.md), [docs/PRD.md §28](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md#L643-L657)).
- **Custom Enclosure**: No 3D-printed or injection-molded shell design; mechanical packaging is deferred until ergonomics and controls are physically validated.
- **Battery & Power Management**: No battery integration, charging ICs, fuel gauges, or deep sleep power optimization; prototype is strictly USB-powered.
- **Proprietary Desktop Application**: No companion utility required for basic operation, configuration, or pairing.
- **Advanced Peripheral Features**: No RGB lighting, complex multi-key macro recording engines, Wi-Fi connectivity, over-the-air (OTA) firmware updates, or cloud profile synchronization.

---

## 4. Users

- **Target User**: Single-user personal handheld pointing device for ergonomic desktop/laptop control without requiring a flat physical desk surface.
- **Interaction Model**: Handheld air controller providing mouse navigation via wrist/arm gestures, thumb scrolling, and index/middle finger button actuation.

---

## 5. Constraints & Architectural Boundaries

### 5.1 Hardware Constraints & Protected TBDs
- **Controller Platform**: ESP32-WROOM-32 family on an ESP32 DevKit V1 development board ([ADR-0002](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0002-prototype-platform.md), [docs/HARDWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/HARDWARE.md)).
- **Power Source**: Powered strictly via USB connection during the prototype phase.
- **Hardware Validation Status**: Physical hardware validation is paused pending the arrival of the physical USB cable.
- **Explicit TBD Lock**:
  - Specific GPIO pin assignments remain **TBD** until the exact board pinout and hardware constraints are physically validated.
  - MPU6050 physical mounting orientation vectors remain **TBD**.
  - I2C electrical details (bus pull-up resistors, bus speeds, logic levels) remain **TBD** where the hardware documentation marks them as such.
  - Physical switch/encoder component details (part numbers, actuation force, debouncing parameters, mounting) remain **TBD** where not yet finalized.
  - *Rule*: No board-specific GPIOs or hardware pinout assumptions may be hardcoded or finalized until validated against the physical board and schematics.

### 5.2 Software & Framework Constraints
- **Development Tooling**: Visual Studio Code with PlatformIO ([ADR-0001](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0001-development-environment.md)). Arduino IDE is not supported.
- **Framework Selection**: Marked **TBD**. Arduino vs. ESP-IDF (with NimBLE) remains an unresolved framework decision until a dedicated research and trade-off analysis ADR is completed and accepted in `docs/decisions/`. No assumption is made regarding which framework will be chosen.
- **Architecture Coupling**: `main.cpp` must remain minimal. Application logic must not depend directly on concrete hardware pins or third-party sensor registers ([ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md)).

### 5.3 Operating & Methodology Rules
- **Source of Truth Hierarchy**: As defined in [AGENTRULES.md §2A](file:///d:/Projects/Zephyr%20-%20Airmouse/AGENTRULES.md#L42-L83):
  - `docs/PRD.md` defines product requirements and intended behavior.
  - `docs/` defines detailed engineering specifications (hardware, controls, architecture, firmware, testing).
  - `docs/decisions/` records architectural and technical decisions (ADRs).
  - `.gsd/SPEC.md` defines the current GSD execution specification derived from the engineering sources of truth.
  - `.gsd/ROADMAP.md` and `.gsd/STATE.md` record execution phases, progress, and session memory.
- **Documentation Synchronization Rule**: Mandatory per [AGENTRULES.md §3](file:///d:/Projects/Zephyr%20-%20Airmouse/AGENTRULES.md#L86-L100). Whenever implementation or a technical decision changes something documented by the project, all relevant documentation must be updated in the same change/commit.
- **Empirical Validation**: Work is not complete without captured verification proof (command output, unit test execution, or hardware test logs) per [PROJECT_RULES.md](file:///d:/Projects/Zephyr%20-%20Airmouse/PROJECT_RULES.md). Simulated, mock, or host test success must never be reported as physical hardware verification.

---

## 6. Success Criteria (Definition of Done)

The Prototype V1 execution is complete when all of the following criteria are empirically verified against [docs/PRD.md §32](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md#L752-L770) and [docs/TESTING.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md):

- [ ] **SC-01 (Build Environment)**: The selected PlatformIO target builds successfully without build errors. Any warnings must be reviewed and either resolved or explicitly documented as accepted.
- [ ] **SC-02 (Framework Decision)**: A research ADR comparing Arduino framework vs. ESP-IDF (with NimBLE) is completed, reviewed, and accepted in `docs/decisions/` without prior assumptions.
- [ ] **SC-03 (Modular Core & Unit Tests)**: Modular firmware abstractions (`core/`, `motion/`, `controls/`, `storage/`) compile and pass native host unit tests for deadzone calculation, smoothing filters, sensitivity scaling, and action mapping.
- [ ] **SC-04 (Hardware Bring-up & Diagnostics)**: Once the physical USB cable is connected, ESP32 boots reliably on USB power, I2C scanner detects MPU6050, and serial diagnostics confirm stable telemetry streaming.
- [ ] **SC-05 (Sensor Calibration)**: On-device calibration routine accurately computes gyroscope zero-rate biases and applies offsets to prevent idle drift.
- [ ] **SC-06 (Bluetooth HID Mouse)**: Device advertises, pairs, and establishes a stable BLE HID connection with a host computer, delivering standard mouse movement reports.
- [ ] **SC-07 (Physical Controls)**: Left, Middle, and Right physical button presses register cleanly with debounce logic and trigger standard mouse click events.
- [ ] **SC-08 (Scroll & Recenter)**: Scroll control transmits smooth vertical scroll reports, and the recenter action re-anchors cursor orientation without position jumps.
- [ ] **SC-09 (Persistent Configuration)**: Configuration is stored persistently in NVS, reloads correctly across power cycles, and automatically falls back to safe defaults if corruption is simulated.
- [ ] **SC-10 (Documentation Integrity)**: All engineering docs in `docs/` and ADR logs in `docs/decisions/` are synchronized with the final prototype implementation.

---

*Last updated: 2026-09-25*
