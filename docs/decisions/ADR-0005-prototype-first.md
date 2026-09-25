# ADR-0005: Prototype Before Custom PCB and Enclosure

- Status: Accepted
- Date: 2026-09-25

## Context

The device combines IMU motion, buttons, a scroll/navigation control, wireless HID, and an unconventional handheld form factor. Early assumptions about ergonomics, sensor orientation, control placement, and motion processing are likely to change.

## Decision

Follow a **prototype-first development sequence**:

1. Breadboard/prototype hardware
2. Firmware and motion validation
3. Control and ergonomic validation
4. Freeze electrical requirements
5. Design custom PCB
6. Design enclosure around the validated hardware

Custom PCB and final enclosure work are explicitly deferred until the prototype reaches the required validation milestone.

## Consequences

### Positive
- Reduces risk of locking incorrect assumptions into hardware
- Allows rapid changes to controls and sensor placement
- Makes firmware behavior easier to validate independently

### Negative
- Prototype wiring is less polished
- Some final-device constraints cannot be validated until later

## Constraints

No final PCB pinout or enclosure dimensions should be treated as authoritative during the prototype phase.
