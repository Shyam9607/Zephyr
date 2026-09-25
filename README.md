# Zephyr

A custom ESP32-based wireless air mouse and handheld pointing controller.

The project combines an IMU-driven cursor, physical controls, configurable input mapping, Bluetooth HID, and eventually a custom PCB and enclosure.

> **Status: Prototype / Active Development**

---

## Overview

Air Mouse is a personal hardware and firmware project built around an ESP32-WROOM-32-family development board.

The initial prototype explores a different approach to mouse interaction:

- Motion-based cursor control using an MPU6050 IMU
- Physical left, middle, and right click buttons
- Thumb-accessible scroll control
- Bluetooth HID mouse functionality
- Recenter capability
- Configurable sensitivity and motion behavior
- Configurable control mappings
- Persistent profiles and settings
- No proprietary desktop application required for basic operation

The project is intentionally being developed **prototype-first**. The custom PCB, battery system, and final enclosure will come after the interaction model and firmware have been validated.

---

## Current Hardware

### Prototype Controller

- ESP32 DevKit V1
- ESP32-WROOM-32-family module
- USB connection for development and initial power
- Bluetooth for wireless HID

### Motion Sensor

- MPU6050
- I2C interface
- Gyroscope used as the primary cursor-motion sensor
- Accelerometer available for calibration, orientation, motion detection, and future features

### Controls

Initial control concept:

```text
┌─────────────────────────┐
│    L    M    R          │
│                         │
│       SCROLL            │
│                         │
│    handheld body        │
└─────────────────────────┘
```

Default behavior:

| Control | Default Action |
|---|---|
| L | Left click |
| M | Middle click |
| R | Right click |
| Scroll | Vertical scrolling |
| Recenter | Logical action, physical assignment TBD |

The exact physical components, GPIO assignments, and final mechanical dimensions are still being validated.

---

## Firmware Architecture

The firmware is designed as a modular system:

```text
                 ┌──────────────┐
                 │   MPU6050    │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ IMU Driver   │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ Calibration  │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │   Filtering  │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ Motion Engine│
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ Cursor / HID │
                 └──────┬───────┘
                        │
                        ▼
                    Computer
```

Physical controls follow a separate path:

```text
Buttons / Encoder
       │
       ▼
 Input Manager
       │
       ▼
 Action Mapping
       │
       ▼
 HID / System Action
```

This separation allows controls to be remapped without rewriting the hardware layer.

---

## Development Environment

The project uses:

- **VS Code**
- **PlatformIO**
- **C/C++**
- ESP32 development toolchain

Arduino IDE is **not required**.

The project is designed to work well with command-line and AI-assisted development workflows.

Potential development assistants include:

- Claude Code
- Codex
- Antigravity
- Traycer
- GSD
- Ralph

These tools are contributors to the workflow, not sources of truth.

**The repository documentation is the source of truth.**

---

## Repository Structure

```text
air-mouse/
│
├── AGENTRULES.md
├── PRD.md
├── README.md
├── .gitignore
│
├── docs/
│   ├── CONTROLS.md
│   ├── HARDWARE.md
│   ├── ARCHITECTURE.md
│   ├── FIRMWARE.md
│   ├── TESTING.md
│   │
│   ├── decisions/
│   │   └── ADR-*.md
│   │
│   └── specs/
│
├── datasheets/
│   ├── esp32/
│   ├── mpu6050/
│   ├── encoder/
│   └── other/
│
├── firmware/
│   ├── src/
│   ├── include/
│   ├── lib/
│   └── test/
│
├── hardware/
│   ├── prototype/
│   ├── schematic/
│   ├── pcb/
│   └── bom/
│
├── enclosure/
│   ├── cad/
│   ├── prototypes/
│   └── dimensions/
│
├── tools/
│   ├── calibration/
│   ├── diagnostics/
│   └── utilities/
│
├── scripts/
│   ├── build/
│   ├── flash/
│   └── debug/
│
└── tests/
    ├── unit/
    ├── integration/
    └── hardware/
```

---

## Documentation First

Documentation is treated as part of the engineering system.

| Document | Purpose |
|---|---|
| `PRD.md` | Product requirements |
| `AGENTRULES.md` | Rules for human and AI contributors |
| `docs/CONTROLS.md` | Physical controls and interaction behavior |
| `docs/HARDWARE.md` | Hardware source of truth |
| `docs/ARCHITECTURE.md` | System architecture |
| `docs/FIRMWARE.md` | Firmware implementation rules |
| `docs/TESTING.md` | Testing strategy |
| `docs/decisions/` | Architecture and engineering decisions |
| `docs/specs/` | Detailed technical specifications |

### Documentation synchronization

**If something changes, every relevant document must be updated.**

For example, changing a GPIO may affect:

- hardware documentation
- firmware documentation
- architecture
- tests
- an ADR
- the PRD/specification

Documentation drift is considered a project defect.

---

## Development Philosophy

The project follows these principles:

1. **Verify hardware before assuming it.**
2. **Prototype before committing to PCB/enclosure decisions.**
3. **Keep firmware modular.**
4. **Separate physical inputs from logical actions.**
5. **Keep configuration versioned and persistent.**
6. **Test what can be tested automatically.**
7. **Never claim hardware validation that was not actually performed.**
8. **Document decisions.**
9. **Keep documentation synchronized with implementation.**
10. **Prefer explicit unknowns over invented answers.**

---

## Project Status

### Planning

- [x] Product requirements drafted
- [x] Control model drafted
- [x] Hardware architecture drafted
- [x] Firmware architecture drafted
- [x] Testing strategy drafted
- [x] Initial ADRs created
- [x] Agent rules created
- [ ] Detailed technical specifications
- [ ] PlatformIO project initialization

### Hardware

- [x] ESP32 DevKit V1 selected for prototype
- [x] MPU6050 selected for initial IMU
- [ ] USB cable / development connection verified
- [ ] Exact board revision verified
- [ ] USB-UART chip verified
- [ ] GPIO map finalized
- [ ] MPU6050 wiring verified
- [ ] Physical controls finalized

### Firmware

- [ ] PlatformIO environment
- [ ] ESP32 boot test
- [ ] Serial diagnostics
- [ ] MPU6050 detection
- [ ] Raw IMU data
- [ ] Calibration
- [ ] Filtering
- [ ] Motion engine
- [ ] Bluetooth HID
- [ ] Cursor control
- [ ] Button input
- [ ] Scroll input
- [ ] Recenter
- [ ] Persistent configuration
- [ ] Profiles

### Hardware Evolution

- [ ] Prototype validation
- [ ] Electrical requirements frozen
- [ ] Custom schematic
- [ ] Custom PCB
- [ ] PCB prototype
- [ ] Final enclosure
- [ ] Battery/power system

---

## License

License to be decided.

---

## Project Philosophy

This is an experimental personal hardware project.

The goal is not simply to make a mouse work.

The goal is to build the entire system properly:

**idea → requirements → architecture → prototype → firmware → testing → PCB → enclosure**

with a repository that remains understandable to both humans and AI agents throughout the process.
