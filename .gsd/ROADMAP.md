---
milestone: v1.0
version: 0.1.0
updated: 2026-09-25T13:25:30+05:30
---

# ROADMAP.md — Zephyr Air Mouse Prototype V1

> **Current Phase:** Phase 1 — Architecture & Framework Trade-off Research
> **Status:** ⬜ Not Started

---

## Must-Haves (from SPEC & PRD §32)

- [ ] Clean PlatformIO build environment with accepted framework ADR (SC-01, SC-02)
- [ ] Modular firmware layout with decoupled core, motion, controls, hid, and storage subsystems (SC-03)
- [ ] Host-testable domain core (deadzone, motion filtering, cursor scaling, action mapping, config schemas) (SC-03)
- [ ] ESP32 DevKit V1 physical hardware bring-up on USB power with MPU6050 I2C sensor stream (SC-04)
- [ ] On-device gyro bias calibration routine preventing idle drift (SC-05)
- [ ] Bluetooth Low Energy HID mouse functionality reporting to host OS (SC-06)
- [ ] Physical Left, Middle, and Right click buttons with software debounce (SC-07)
- [ ] Dedicated scroll control and cursor recentering action (SC-08)
- [ ] On-device persistent NVS configuration with automatic safe fallback on corrupt data (SC-09)
- [ ] Complete documentation synchronization across all repository docs (SC-10)

---

## Phases

### Phase 1: Architecture & Framework Trade-off Research
**Status:** ⬜ Not Started  
**Objective:** Perform a dedicated technical trade-off evaluation comparing Arduino framework vs. ESP-IDF (with NimBLE) on PlatformIO for ESP32-WROOM-32. Evaluate Bluetooth HID capabilities, RAM/flash overhead, library ecosystem, build performance, and long-term maintainability. Record and accept the decision in `docs/decisions/ADR-0009-framework-selection.md` and synchronize `docs/FIRMWARE.md`.  
**Requirements:** REQ-02, NFR-01, CON-01, CON-03  
**Depends on:** None (executable immediately without hardware)

---

### Phase 2: Firmware Scaffolding & Native Domain Core
**Status:** ⬜ Not Started  
**Objective:** Construct the modular PlatformIO project structure (`core/`, `motion/`, `controls/`, `hid/`, `storage/`, `power/`, `diagnostics/`) based on the accepted framework decision. Implement hardware abstraction interfaces, motion filtering math (EMA/low-pass, deadzone thresholds), cursor scaling/acceleration, action mapping, and versioned configuration schemas. Build and execute native host unit tests to verify algorithms independently of physical hardware.  
**Requirements:** REQ-01, REQ-03, REQ-04, REQ-05, REQ-06, REQ-07, NFR-01, NFR-04  
**Depends on:** Phase 1 (executable immediately on host while USB cable is pending)

---

### Phase 3: Hardware Bring-Up, Sensor Pipeline & Bluetooth HID
**Status:** ⬜ Not Started  
**Objective:** Once the physical USB cable arrives, verify physical ESP32 DevKit V1 board pinout and connect MPU6050 over I2C. Flash baseline diagnostic firmware, run I2C bus scanner, stream real-time telemetry over serial monitor, validate zero-rate gyro calibration on hardware, bring up the BLE HID stack, and verify that physical IMU motion drives the mouse cursor on a paired host computer.  
**Requirements:** REQ-08, REQ-09, REQ-10, REQ-14, NFR-02, CON-02, CON-04  
**Depends on:** Phase 2, Physical USB cable connection

---

### Phase 4: Physical Controls, Scrolling & Recentering Integration
**Status:** ⬜ Not Started  
**Objective:** Wire and test physical Left, Middle, and Right buttons with debouncing logic. Integrate the scroll control (Adafruit ANO-style candidate or prototype rotary encoder) to generate vertical HID scroll events. Implement and test the logical recentering action. Connect the action mapping engine to dispatch button, scroll, and recenter actions to the BLE HID pipeline.  
**Requirements:** REQ-11, REQ-12, REQ-13  
**Depends on:** Phase 3

---

### Phase 5: Persistent Configuration, Resilience & Milestone Verification
**Status:** ⬜ Not Started  
**Objective:** Integrate ESP32 NVS non-volatile storage to persist user profiles, motion settings, and button mappings. Test power-cycle retention and simulate data corruption to empirically verify fallback to safe compile-time defaults. Perform full end-to-end regression and latency testing against PRD §32 Definition of Done, and verify complete documentation synchronization across `docs/` and `docs/decisions/`.  
**Requirements:** REQ-06, NFR-02, NFR-03, CON-05, CON-06  
**Depends on:** Phase 4

---

## Progress Summary

| Phase | Description | Status | Plans | Complete |
|---|---|:---:|:---:|:---:|
| 1 | Architecture & Framework Trade-off Research | ⬜ | 0/1 | — |
| 2 | Firmware Scaffolding & Native Domain Core | ⬜ | 0/2 | — |
| 3 | Hardware Bring-Up, Sensor Pipeline & BLE HID | ⬜ | 0/2 | — |
| 4 | Physical Controls, Scrolling & Recentering | ⬜ | 0/2 | — |
| 5 | Persistent Config, Resilience & Verification | ⬜ | 0/1 | — |

---

## Status Icons Key

- ⬜ Not Started
- 🔄 In Progress
- ✅ Complete
- ⏸️ Paused / Awaiting Hardware
- ❌ Blocked
