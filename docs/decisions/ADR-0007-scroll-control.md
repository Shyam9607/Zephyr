# ADR-0007: Scroll and Directional Control Candidate

- Status: Proposed
- Date: 2026-09-25

## Context

The physical design calls for a thumb-accessible scroll control below or near the left-click area. The user is considering an Adafruit ANO-style directional navigation/scroll-wheel control.

## Decision

Treat an **Adafruit ANO-style directional navigation and scroll-wheel control** as the primary prototype candidate, pending exact component selection and interface verification.

The firmware should not hard-code the physical control into the action system. Rotation and directional inputs should produce abstract input events that can be remapped.

## Consequences

### Positive
- Compact control concept
- Supports scrolling plus additional directional/center actions
- Fits the desired handheld control layout

### Negative
- Exact component/interface still needs verification
- Mechanical mounting and availability need validation

## Constraints

The selected part, electrical interface, pin requirements, and mechanical dimensions must be recorded before final hardware implementation.
