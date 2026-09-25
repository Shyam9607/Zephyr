---
updated: 2026-09-25T13:25:40+05:30
---

# Project State — Zephyr Air Mouse

## Current Position

**Milestone:** v1.0 (Prototype V1)  
**Phase:** Ready for Phase 1 — Architecture & Framework Trade-off Research  
**Status:** Ready to plan  
**Plan:** None  

## Last Action

Initialized Zephyr project under GSD framework:
- Formally finalized [.gsd/SPEC.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/SPEC.md) reflecting [docs/PRD.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/PRD.md) and all engineering constraints.
- Generated [.gsd/REQUIREMENTS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/REQUIREMENTS.md) with full traceability matrix.
- Created [.gsd/ROADMAP.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/ROADMAP.md) detailing 5 sequential phases from trade-off research through host testing to hardware validation.

## Next Steps

1. Run `/discuss-phase 1` or `/plan 1` to create the execution plan for Phase 1 (Architecture & Framework Trade-off Research).
2. Execute Phase 1: Produce and accept `docs/decisions/ADR-0009-framework-selection.md` comparing Arduino framework vs. ESP-IDF (with NimBLE) for PlatformIO.
3. Synchronize [docs/FIRMWARE.md](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md) with the accepted framework decision.

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
