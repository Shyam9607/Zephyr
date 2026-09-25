# ADR-0008: On-Device Configuration

- Status: Accepted
- Date: 2026-09-25

## Context

The device should behave as a standalone controller. Basic operation should not depend on a proprietary desktop application.

At the same time, the project needs configurable sensitivity, mappings, profiles, and other behavior.

## Decision

Store runtime configuration and user profiles **on the device** using a versioned persistent configuration system.

The configuration layer will support:
- schema/version validation
- defaults
- invalid/corrupt configuration recovery
- persistent profiles
- configurable motion parameters
- configurable control mappings

A desktop configuration utility may be added later, but it is not required for basic operation.

## Consequences

### Positive
- Device remains self-contained
- Configuration survives host changes
- Firmware can evolve through explicit schema versions

### Negative
- Configuration migration must be handled carefully
- Advanced editing may eventually benefit from a companion tool

## Constraints

Configuration data must never be trusted blindly. Invalid or incompatible data must fall back safely to known defaults.
