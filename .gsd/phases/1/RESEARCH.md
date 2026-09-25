---
phase: 1
researched_at: 2026-09-25
discovery_level: 3
---

# Phase 1 Research: Architecture & Framework Trade-off (Arduino vs. ESP-IDF)

## Objective

Determine the optimal PlatformIO firmware framework for the Zephyr Air Mouse (ESP32-WROOM-32) by conducting architectural research and establishing empirical local benchmark spikes comparing the Arduino framework against ESP-IDF (with NimBLE).

---

## Discovery Level & Scope

**Level 3 — Deep Dive / Architectural Decision**  
Framework selection determines project build configuration, Bluetooth HID latency and reliability, available RAM/flash headroom, and driver integration. While an abstract Hardware Abstraction Layer (HAL) will isolate domain logic from framework APIs, the underlying platform dictates firmware stability and maintenance overhead.

---

## Prioritized Evaluation Criteria

Per the agreement in [.gsd/DECISIONS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/DECISIONS.md), candidates are evaluated strictly in the following priority order:

1. **Bluetooth HID device capability & implementation quality**:
   - Standard 4-byte/5-byte BLE mouse reports (X/Y relative displacement, Left/Middle/Right buttons, vertical scroll wheel).
   - Feasibility of future composite HID reports (keyboard shortcuts, media keys).
   - Pairing, bonding, connection parameters, and fast reconnection stability across Windows, macOS, and Linux hosts.
   - API maturity, stability, and implementation complexity.
2. **Resource usage & available headroom**:
   - Firmware binary flash size against standard 4MB ESP32 flash partition table.
   - Static RAM and runtime heap consumption (critical for sensor FIFO buffers and motion processing).
3. **Long-term maintainability & architectural fit**:
   - Upstream maintenance, documentation quality, and clean alignment with ESP32 FreeRTOS tasks.
4. **Development velocity & library ecosystem**:
   - Availability and reliability of I2C drivers for MPU6050 and NVS storage drivers.
5. **Build/configuration complexity & reproducibility**:
   - PlatformIO toolchain reproducibility from clean checkout, clean build duration, and incremental build cycle.
6. **Host-testability & framework independence**:
   - Ease of decoupling domain logic into pure C++ so that native PC unit tests run without embedded headers.

---

## Architectural Comparison of Candidates

### Candidate A: Arduino Framework (with NimBLE-Arduino)
- **Overview**: High-level C++ abstraction layer running on top of ESP-IDF FreeRTOS.
- **Bluetooth Stack**:
  - *Default (Bluedroid)*: High flash consumption (~1.2 MB+) and large RAM footprint (~100 KB+).
  - *NimBLE Alternative (`NimBLE-Arduino`)*: Drop-in replacement for BLE stack reducing flash footprint by ~50% and RAM usage by ~70%, offering fine-grained control over connection parameters.
- **Pros**:
  - Extremely rapid prototyping and vast ecosystem of community peripheral libraries (MPU6050, Adafruit ANO, Preferences/NVS).
  - Well-understood `ESP32-BLE-Mouse` (or `NimBLE-Arduino` equivalent) APIs for rapid validation.
- **Cons**:
  - Layer of abstraction above ESP-IDF can introduce hidden latency or make fine-grained FreeRTOS task affinity tuning less transparent.
  - Library versions can occasionally conflict with underlying ESP-IDF versions packaged in the Arduino-ESP32 core.

### Candidate B: ESP-IDF Framework (Native with NimBLE)
- **Overview**: Official Espressif development framework providing direct access to the ESP32 hardware, FreeRTOS kernel, and native drivers.
- **Bluetooth Stack**:
  - Native Espressif NimBLE stack or `esp_hid` component.
- **Pros**:
  - Leanest possible resource footprint, deterministic task scheduling, direct interrupt and timer management.
  - Native support for custom HID report descriptors and power management modes.
  - Highly robust upstream maintenance directly from the chip manufacturer.
- **Cons**:
  - Substantially higher configuration complexity (CMake, Kconfig/sdkconfig, PlatformIO integration).
  - Longer initial compilation times and higher boilerplate for I2C and NVS initialization.

---

## Empirical Spike Protocol (Option B)

To avoid relying on generic internet claims, Phase 1 will execute local minimal spikes under `scratch/spikes/`:
- **Spike A (`spike-arduino-nimble`)**: Minimal PlatformIO environment targeting ESP32 DevKit V1 with `framework = arduino` and `NimBLE-Arduino` establishing a stub BLE mouse HID service.
- **Spike B (`spike-espidf-nimble`)**: Minimal PlatformIO environment targeting ESP32 DevKit V1 with `framework = espidf` and native NimBLE establishing a stub BLE mouse HID service.

### Metrics to Capture:
- Clean build time (seconds from clean checkout)
- Incremental build time (seconds after touching a single source file)
- Flash memory utilization (bytes and percentage of partition)
- Static RAM utilization (bytes and percentage of DRAM)
- Toolchain, core, and package versions reported by PlatformIO CLI (`pio.exe`)

---

## Architectural Constraint: HAL Framework Independence

Per [ADR-0006](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/decisions/ADR-0006-modular-firmware.md) and [.gsd/DECISIONS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/DECISIONS.md), domain logic must NEVER directly include Arduino (`Arduino.h`) or ESP-IDF (`esp_system.h`, `freertos/FreeRTOS.h`) headers.

```text
┌────────────────────────────────────────────────────────┐
│     Domain Logic (Motion, Filtering, Controls, Config) │
└───────────────────────────┬────────────────────────────┘
                            │ Pure C++ Interfaces
                            ▼
┌────────────────────────────────────────────────────────┐
│  HAL Interfaces: IImuDriver, IHidMouse, IStorageService │
└───────────────────────────┬────────────────────────────┘
                            │ Concrete Implementation
                            ▼
┌────────────────────────────────────────────────────────┐
│   Hardware Adapter (Arduino or ESP-IDF Implementation)  │
└────────────────────────────────────────────────────────┘
```

This guarantees that:
1. All mathematical algorithms, deadzone handling, sensitivity scaling, and state transitions are 100% testable on PC via native unit tests.
2. The framework choice remains purely an infrastructure detail that can be replaced without rewriting application behavior.

---

## Recommendations for Phase 1 Plans

- **Plan 1.1**: Construct minimal comparison build spikes in `scratch/spikes/`, run PlatformIO builds, and record rigorous local empirical measurements (build time, flash, RAM).
- **Plan 1.2**: Synthesize architectural analysis, HID capability assessment, and empirical spike data into `docs/decisions/ADR-0009-framework-selection.md`.
- **Plan 1.3**: Synchronize [docs/FIRMWARE.md §4](file:///d:/Projects/Zephyr%20-%20Airmouse/docs/FIRMWARE.md#L55-L75), update [.gsd/REQUIREMENTS.md](file:///d:/Projects/Zephyr%20-%20Airmouse/.gsd/REQUIREMENTS.md) (REQ-02 complete), and update project state.
