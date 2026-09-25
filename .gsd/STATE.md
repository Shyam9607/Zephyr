---
updated: 2026-09-25T13:25:40+05:30
---

# Project State — Zephyr Air Mouse

## Current Position

**Milestone:** v1.0 (Prototype V1)  
**Phase:** Phase 1 — Architecture & Framework Trade-off Research  
**Status:** Discussion completed; ready to plan  
**Plan:** None  

## Last Action

Completed `/discuss-phase 1` for Architecture & Framework Trade-off Research:
- Chose Option B (Analytical + Minimal Build Spikes) with empirical local PlatformIO metrics.
- Prioritized evaluation criteria hierarchy (1: HID capability, 2: Resource headroom, 3: Maintainability, 4: Velocity/ecosystem, 5: Complexity, 6: Host-testability).
- Established mandatory framework independence constraint (abstract C++ interfaces encapsulating HAL).
- Documented decisions and deliverables in [.gsd/DECISIONS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/DECISIONS.md).

## Next Steps

1. Run `/plan 1` to create detailed execution plans for Phase 1.
2. Execute Phase 1: Setup minimal PlatformIO spikes, collect empirical build/flash/RAM metrics, evaluate BLE HID quality, draft and accept `docs/decisions/ADR-0009-framework-selection.md`, and synchronize [docs/FIRMWARE.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md#L55-L75).

## Active Decisions

All established architectural decisions are recorded in [docs/decisions/](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/):

| Decision | Choice | Status | Affects |
|---|---|---|---|
| [ADR-0001](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0001-development-environment.md) | VS Code + PlatformIO build environment | Accepted | All firmware tooling |
| [ADR-0002](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0002-prototype-platform.md) | ESP32-WROOM-32 family DevKit V1 board | Accepted | Hardware baseline |
| [ADR-0003](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0003-imu.md) | MPU6050 (gyro primary, accel secondary) over I2C | Accepted | Motion subsystem |
| [ADR-0004](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0004-bluetooth-hid.md) | Bluetooth Low Energy HID mouse interface | Accepted | Host communication |
| [ADR-0005](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0005-prototype-first.md) | Prototype-first; custom PCB and enclosure deferred | Accepted | Project scope |
| [ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md) | Modular firmware architecture with strict interfaces | Accepted | Firmware architecture |
| [ADR-0007](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0007-scroll-control.md) | Adafruit ANO-style navigation candidate for scroll | Proposed | Controls subsystem |
| [ADR-0008](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0008-on-device-configuration.md) | On-device persistent NVS configuration & profiles | Accepted | Storage subsystem |
| ADR-0009 (Pending) | Framework Selection (Arduino vs. ESP-IDF/NimBLE) | TBD | Phase 1 deliverable |

## Blockers

- None currently blocking Phase 1 or Phase 2.
- *Hardware Note*: Physical hardware validation in Phase 3 is pending arrival of the physical USB cable. Software scaffolding, math/filter algorithms, and host tests (Phases 1 & 2) proceed without hardware dependency.

## Concerns

- Ensure framework trade-off analysis in Phase 1 thoroughly weighs NimBLE memory efficiency against Arduino community library convenience.
- Maintain strict documentation synchronization across [docs/](file:///d:/Projects/Zephyr%20-%20Airmouse/docs) for any and all technical decisions.

## Session Context

- Project is initialized in GSD.
- All hardware pinout, orientation, and electrical details remain protected as TBD.
