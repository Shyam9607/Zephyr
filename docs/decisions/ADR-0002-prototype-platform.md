# ADR-0002: Initial Prototype Platform

- Status: Accepted
- Date: 2026-09-25

## Context

The first hardware revision should validate the interaction model and firmware architecture before committing to a custom PCB or enclosure.

The available prototype platform is an ESP32 DevKit-style board containing an ESP32-WROOM-32-family module.

## Decision

Use the **ESP32-WROOM-32-family DevKit-style board** as the initial prototype controller.

The exact DevKit revision, USB-UART bridge, regulator, and board pinout must be verified from the physical board and its documentation before assigning production GPIOs.

## Consequences

### Positive
- Fast breadboard prototyping
- USB programming and serial debugging
- Integrated Bluetooth and Wi-Fi
- Easy replacement during early experiments

### Negative
- Prototype wiring is less compact than the final device
- Exact third-party DevKit details may differ between boards
- Final PCB design will require a later hardware-specific decision

## Constraints

No GPIO assignment may be treated as final until the exact board and all relevant pin restrictions have been verified.
