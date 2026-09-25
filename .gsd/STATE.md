---
updated: 2026-09-25T13:25:40+05:30
---

# Project State — Zephyr Air Mouse

## Current Position

**Milestone:** v1.0 (Prototype V1)  
**Phase:** Phase 1 — Architecture & Framework Trade-off Research  
**Status:** Plan 1.1 complete; ready for Plan 1.2  
**Plan:** Plan 1.2 (Framework Evaluation & ADR-0009 Formulation)  

## Last Action

Completed Plan 1.1 (Minimal PlatformIO Build Spikes & Local Empirical Benchmarks):
- Established architecturally equivalent BLE HID mouse spikes (50-byte report map, 4-byte report payload with 3 buttons L/M/R, X/Y movement, wheel, DIS, and mouse appearance) in `scratch/spikes/arduino-nimble` and `scratch/spikes/espidf-nimble`.
- Executed local PlatformIO builds and captured empirical measurements:
  - Arduino + NimBLE: Clean 19.43s, Incremental 4.04s, Flash 591,501 bytes (45.1%), Static RAM 35,684 bytes (10.9%).
  - ESP-IDF + NimBLE: Clean 155.81s, Incremental 5.20s, Flash 446,237 bytes (42.6%), Static RAM 27,132 bytes (8.3%).
- Documented tooling quirk: ESP-IDF strictly rejects whitespace in workspace directory paths (`espidf.py`), resolved via virtual drive mapping `subst Z:`.
- Documented complete benchmark results in `scratch/spikes/BENCHMARK_REPORT.md` and produced `.gsd/phases/1/1.1-SUMMARY.md`.

## Next Steps

1. Proceed to Plan 1.2 to analyze trade-offs across the 6 prioritized criteria and draft `docs/decisions/ADR-0009-framework-selection.md`.
2. Proceed to Plan 1.3 to synchronize [docs/FIRMWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md), update decision indexes, and close Phase 1.

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
