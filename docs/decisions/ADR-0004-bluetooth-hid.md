# ADR-0004: Wireless Input Protocol

- Status: Accepted
- Date: 2026-09-25

## Context

The finished controller should operate as a normal pointing device without requiring proprietary desktop software for basic mouse functionality.

## Decision

Use **Bluetooth HID** as the primary wireless host interface.

The minimum required HID behavior is mouse input:
- cursor movement
- left click
- middle click
- right click
- scroll

Additional HID keyboard/media functionality may be added later.

## Consequences

### Positive
- Host computers can treat the device as a standard input device
- No custom desktop driver is required for the basic mouse function
- Keeps the architecture centered on firmware-side configuration

### Negative
- Bluetooth HID connection and compatibility need dedicated testing
- Host behavior can vary across operating systems
- Pairing/reconnection behavior must be designed explicitly

## Constraints

The HID layer must remain separate from physical control logic so the same actions can later target other interfaces if required.
