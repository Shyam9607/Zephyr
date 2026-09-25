# DECISIONS.md — Architecture Decision Records

> **Notice**: Per [AGENTRULES.md §2A](file:///d:/Projects/Zephyr%20-%20Airmouse/AGENTRULES.md), canonical ADRs for the Zephyr project reside in [docs/decisions/](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/).
> This file acts as the GSD decision log and index pointing to authoritative ADRs.

---

## Authoritative ADR Index

| ADR ID | Title | Status | Date | Primary Scope |
|---|---|:---:|:---:|---|
| [ADR-0001](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0001-development-environment.md) | Development Environment | Accepted | 2026-09-25 | VS Code + PlatformIO |
| [ADR-0002](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0002-prototype-platform.md) | Initial Prototype Platform | Accepted | 2026-09-25 | ESP32-WROOM-32 DevKit V1 |
| [ADR-0003](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0003-imu.md) | Initial IMU | Accepted | 2026-09-25 | MPU6050 over I2C |
| [ADR-0004](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0004-bluetooth-hid.md) | Wireless Input Protocol | Accepted | 2026-09-25 | Bluetooth Low Energy HID |
| [ADR-0005](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0005-prototype-first.md) | Prototype Before Custom PCB and Enclosure | Accepted | 2026-09-25 | Prototype-first sequence |
| [ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md) | Modular Firmware Architecture | Accepted | 2026-09-25 | Modular subsystem design |
| [ADR-0007](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0007-scroll-control.md) | Scroll and Directional Control Candidate | Proposed | 2026-09-25 | Adafruit ANO navigation wheel |
| [ADR-0008](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0008-on-device-configuration.md) | On-Device Configuration | Accepted | 2026-09-25 | Versioned NVS schemas |
| ADR-0009 | Framework Selection (Arduino vs. ESP-IDF) | Pending | Planned | Scheduled for Phase 1 |

---

## Phase 1 Discussion & Decisions

**Date:** 2026-09-25

### Scope & Methodology
- **Approach**: Option B (Analytical + Minimal Build Spikes). Architectural research will be paired with minimal, comparable PlatformIO build spikes for Arduino and ESP-IDF to establish empirical measurements locally.
- **Spike Constraints**: Spikes are strictly minimal comparison environments to establish comparable metrics and verify BLE HID feasibility — not full duplicate implementations.
- **Empirical Metrics**:
  - Clean build time
  - Incremental build time
  - Firmware flash size
  - Static RAM usage / reported memory footprint
  - Dependency footprint and configuration complexity
  - Reproducibility from a clean checkout
- **Measurement Integrity**: Do not treat generic published memory numbers as authoritative. Record configuration, framework/core/PlatformIO versions, build flags, and measurement methodology alongside empirical data.

### Evaluation Criteria Hierarchy
Decisions will be evaluated strictly in this prioritized order:
1. Bluetooth HID device capability and implementation quality (BLE mouse X/Y, L/M/R click, scroll, future keyboard/media reports, pairing/reconnection, API stability).
2. Resource usage and available RAM/flash headroom.
3. Long-term maintainability and architectural fit.
4. Development velocity and library ecosystem.
5. Build/configuration complexity and reproducibility.
6. Host-testability and ability to keep domain logic framework-independent.

### Architectural Constraint: Framework Independence
- Domain and application logic (motion filtering, cursor translation, controls, action mapper, configuration schemas) must be strictly decoupled from vendor/framework APIs behind abstract C++ interfaces (e.g. `IImuDriver`, `IHidMouse`, `IStorageService`, `IInputDevice`).
- Framework selection must remain isolated to the hardware adapter/driver layer so that "domain logic remains testable on native host platforms without requiring the selected embedded framework."

### Deliverables Mandated
- Comprehensive `docs/decisions/ADR-0009-framework-selection.md` capturing evaluation criteria, configurations tested, empirical measurements, methodology, limitations, rejected alternatives, and final rationale.
- Synchronization of [docs/FIRMWARE.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md#L55-L75) and any related project documentation.

---

*Last updated: 2026-09-25*
