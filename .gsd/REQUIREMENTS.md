---
milestone: v1.0
updated: 2026-09-25T13:25:00+05:30
---

# Requirements — Zephyr Air Mouse Prototype V1

## Overview

Formal requirements derived directly from [.gsd/SPEC.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/SPEC.md), [docs/PRD.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md), [docs/ARCHITECTURE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/ARCHITECTURE.md), [docs/FIRMWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md), [docs/HARDWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/HARDWARE.md), [docs/CONTROLS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md), [docs/TESTING.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md), and [ADRs](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/).

---

## Functional Requirements

| ID | Requirement | Source | Phase | Status |
|----|-------------|--------|-------|--------|
| REQ-01 | Establish modular firmware layout separating hardware drivers, motion, controls, hid, storage, and diagnostics | SPEC Goal 1, [ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md) | 2 | Pending |
| REQ-02 | Complete formal Trade-off Analysis ADR evaluating Arduino vs. ESP-IDF (NimBLE) for PlatformIO build framework | SPEC SC-02, [docs/FIRMWARE.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md#L55-L75) | 1 | Pending |
| REQ-03 | Implement motion filter with deadzone thresholds to eliminate resting jitter and micro-tremors | SPEC Goal 3, [docs/ARCHITECTURE.md §3](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/ARCHITECTURE.md#L38-L86) | 2 | Pending |
| REQ-04 | Implement cursor engine translating angular velocity into X/Y displacement with configurable sensitivity and acceleration | SPEC Goal 3, [docs/PRD.md §5](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md#L125-L150) | 2 | Pending |
| REQ-05 | Implement action mapping system decoupling physical hardware inputs from emitted logical actions | SPEC Goal 1, [docs/CONTROLS.md §6](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md#L140-L180) | 2 | Pending |
| REQ-06 | Implement on-device persistent configuration storage in NVS with versioned schema and defaults fallback on corruption | SPEC Goal 8, [ADR-0008](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0008-on-device-configuration.md) | 2 | Pending |
| REQ-07 | Implement host-based unit tests for math, filters, action mapper, and config schema validation | SPEC Goal 9, [docs/TESTING.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md#L55-L75) | 2 | Pending |
| REQ-08 | Implement MPU6050 I2C sensor driver acquiring 3-axis gyro and 3-axis accelerometer readings | SPEC Goal 2, [ADR-0003](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0003-imu.md) | 3 | Pending |
| REQ-09 | Implement on-device calibration routine computing gyroscope zero-rate bias offsets | SPEC Goal 2, [docs/TESTING.md §6](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md#L103-L119) | 3 | Pending |
| REQ-10 | Implement Bluetooth Low Energy HID mouse service transmitting standard movement and click reports to host OS | SPEC Goal 4, [ADR-0004](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0004-bluetooth-hid.md) | 3 | Pending |
| REQ-11 | Implement physical button input handling with software debounce for Left, Middle, and Right mouse buttons | SPEC Goal 5, [docs/CONTROLS.md §3](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md#L46-L100) | 4 | Pending |
| REQ-12 | Implement scroll control translating rotary/directional events into HID vertical scroll reports | SPEC Goal 6, [ADR-0007](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0007-scroll-control.md) | 4 | Pending |
| REQ-13 | Implement cursor recentering logical action to reset orientation reference without position jumps | SPEC Goal 7, [docs/CONTROLS.md §5](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md) | 4 | Pending |
| REQ-14 | Implement serial diagnostic reporting for telemetry, connection status, and error logs | SPEC Goal 1, [docs/FIRMWARE.md §16](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md) | 3 | Pending |

---

## Non-Functional Requirements

| ID | Requirement | Category | Phase | Status |
|----|-------------|----------|-------|--------|
| NFR-01 | Clean build: PlatformIO target compiles without errors; warnings reviewed and resolved or documented | Build & Tooling | 1, 2 | Pending |
| NFR-02 | Pipeline latency: Sensor acquisition to HID dispatch latency kept minimal (<15ms target) | Performance | 3, 5 | Pending |
| NFR-03 | Standalone operation: Operates as standard Bluetooth HID without requiring proprietary desktop companion software | Architecture | 3, 5 | Pending |
| NFR-04 | Host unit testability: Core algorithmic logic testable on host without requiring physical hardware | Quality & CI | 2 | Pending |

---

## Constraints

| ID | Constraint | Source | Impact |
|----|------------|--------|--------|
| CON-01 | VS Code + PlatformIO build environment required; Arduino IDE not supported | [ADR-0001](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0001-development-environment.md) | All firmware tooling |
| CON-02 | ESP32 DevKit V1 (ESP32-WROOM-32) on USB power only; no battery or charging circuits in V1 | [ADR-0002](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0002-prototype-platform.md), SPEC §3 | Hardware scope |
| CON-03 | Framework choice (Arduino vs ESP-IDF) remains TBD until dedicated research ADR is accepted | SPEC §5.2, [docs/FIRMWARE.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md) | Build configuration |
| CON-04 | Hardware TBDs locked: GPIO assignments, MPU6050 orientation, I2C electricals, switch mechanics TBD until physical cable connects | SPEC §5.1, [docs/HARDWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/HARDWARE.md) | Pin and driver configs |
| CON-05 | Mandatory documentation synchronization across all touched `docs/` files in the same commit | [AGENTRULES.md §3](file:///d:/Projects/Zephyr%20-%20Airmouse/AGENTRULES.md), SPEC §5.3 | Every git commit |
| CON-06 | Empirical validation required: Proof captured from test runner or physical hardware; no simulated claims of physical success | [PROJECT_RULES.md](file:///d:/Projects/Zephyr%20-%20Airmouse/PROJECT_RULES.md), SPEC §5.3 | Definition of done |

---

## Traceability Matrix

| Requirement | SPEC Goal | Primary Docs | Phase |
|-------------|-----------|--------------|-------|
| REQ-01 | Goal 1 | [docs/FIRMWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md), [ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md) | Phase 2 |
| REQ-02 | SC-02 | [docs/FIRMWARE.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md) | Phase 1 |
| REQ-03 | Goal 3 | [docs/ARCHITECTURE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/ARCHITECTURE.md) | Phase 2 |
| REQ-04 | Goal 3 | [docs/PRD.md §5](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md) | Phase 2 |
| REQ-05 | Goal 1 | [docs/CONTROLS.md §6](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md) | Phase 2 |
| REQ-06 | Goal 8 | [ADR-0008](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0008-on-device-configuration.md) | Phase 2 |
| REQ-07 | Goal 9 | [docs/TESTING.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md) | Phase 2 |
| REQ-08 | Goal 2 | [docs/HARDWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/HARDWARE.md), [ADR-0003](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0003-imu.md) | Phase 3 |
| REQ-09 | Goal 2 | [docs/TESTING.md §6](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/TESTING.md) | Phase 3 |
| REQ-10 | Goal 4 | [ADR-0004](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0004-bluetooth-hid.md) | Phase 3 |
| REQ-11 | Goal 5 | [docs/CONTROLS.md §3](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md) | Phase 4 |
| REQ-12 | Goal 6 | [docs/CONTROLS.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md), [ADR-0007](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0007-scroll-control.md) | Phase 4 |
| REQ-13 | Goal 7 | [docs/CONTROLS.md §5](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/CONTROLS.md) | Phase 4 |
| REQ-14 | Goal 1 | [docs/FIRMWARE.md §16](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md) | Phase 3 |
