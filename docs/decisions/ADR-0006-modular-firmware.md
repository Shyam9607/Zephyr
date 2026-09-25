# ADR-0006: Modular Firmware Architecture

- Status: Accepted
- Date: 2026-09-25

## Context

The project is intended to grow from a basic air mouse into a highly configurable controller with profiles, remapping, macros, different interaction types, and potentially additional HID functions.

A monolithic firmware implementation would make experimentation and agent-assisted development harder.

## Decision

Use a modular firmware architecture with clear boundaries between:

- hardware drivers
- IMU and sensor processing
- motion processing
- physical controls
- action mapping
- HID
- profiles/configuration
- persistent storage
- diagnostics
- power management

The application layer should depend on interfaces/modules rather than embedding hardware-specific behavior throughout the codebase.

## Consequences

### Positive
- Easier testing
- Easier hardware replacement
- Easier AI-assisted development
- Clearer ownership of responsibilities
- Supports future control remapping and profiles

### Negative
- More files and interfaces
- Requires discipline around dependency boundaries

## Constraints

`main.cpp` should remain small. Hardware-specific constants must not leak unnecessarily into application logic.
