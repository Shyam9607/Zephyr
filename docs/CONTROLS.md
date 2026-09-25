# Air Mouse Controls Specification

**Project:** Personal ESP32 Air Mouse  
**Document:** Controls Specification  
**Version:** 0.1.0  
**Status:** Draft

---

## 1. Purpose

This document defines the physical controls, their intended default behavior, interaction semantics, and future customization requirements for the Air Mouse.

The controls are designed around a compact handheld air-mouse form factor rather than a conventional desktop mouse.

---

## 2. Initial Physical Layout

The current physical concept consists of:

- Three primary buttons arranged horizontally near the upper/front portion of the device.
- One rotary scroll control positioned below the primary buttons.
- The scroll control is intended to be reachable naturally by the user's thumb.
- The final enclosure geometry and exact dimensions are TBD.

### Current layout

```text
             ┌───┐ ┌───┐ ┌───┐
             │ L │ │ M │ │ R │
             └───┘ └───┘ └───┘


                    ◯
                  SCROLL


              AIR MOUSE BODY
```

The layout is a functional concept and is not yet the final mechanical design.

---

# 3. Primary Mouse Buttons

## 3.1 Left Button

**Label:** L

**Default action:**
- Mouse Left Click

**Future configurable actions may include:**
- Keyboard key
- Keyboard combination
- Macro
- Media control
- Profile action
- Firmware-defined function

---

## 3.2 Middle Button

**Label:** M

**Default action:**
- Mouse Middle Click

**Potential future actions:**
- Recenter
- Keyboard key
- Keyboard combination
- Macro
- Media control
- Profile switching
- Firmware-defined function

The middle button is intentionally retained as a dedicated physical control because middle-click is useful for normal desktop and browser interaction.

---

## 3.3 Right Button

**Label:** R

**Default action:**
- Mouse Right Click

**Future configurable actions may include:**
- Keyboard key
- Keyboard combination
- Macro
- Media control
- Profile action
- Firmware-defined function

---

# 4. Scroll Control

The primary scrolling mechanism is a rotary encoder / scroll wheel.

## Default behavior

| Action | Function |
|---|---|
| Clockwise rotation | Scroll down |
| Counter-clockwise rotation | Scroll up |

The direction may be configurable in firmware.

## Future behavior

The scroll control MAY support:

- Adjustable scroll sensitivity
- Scroll acceleration
- Horizontal scrolling
- Alternate scroll modes
- Application-specific behavior
- Profile-specific behavior
- Custom actions

---

# 5. Scroll Control Placement

The scroll control is intentionally positioned below the primary mouse buttons.

The intended ergonomic goal is:

- Index/middle fingers interact with the primary buttons.
- Thumb naturally reaches the scroll control.
- User should not need to significantly reposition their hand to scroll.
- Rotation should be possible without losing a stable grip.

Exact placement will be validated through physical prototyping.

---

# 6. Recenter Control

A dedicated recenter control is planned but has not yet been assigned a final physical location.

## Function

Activating recenter should:

1. Capture the current sensor reference state.
2. Treat the current orientation as the new neutral position.
3. Reset relative cursor movement around that reference.
4. Avoid requiring a device restart.

Potential implementations:

- Dedicated physical button
- Long press of an existing button
- Combination input
- Configurable button action

**Status:** TBD

---

# 7. Button Interaction Types

The firmware SHOULD support multiple interaction types.

Potential interaction types:

### Single Press

```text
PRESS → execute assigned action
```

### Hold

```text
PRESS
  ↓
HOLD
  ↓
execute hold behavior
```

### Long Press

A configurable duration may trigger a secondary action.

### Double Press

Two presses within a configurable time window may trigger an alternate action.

### Press + Release

Some actions may execute specifically on release.

The final interaction model is TBD and should be implemented only where it improves usability.

---

# 8. Button Mapping

Physical controls should be logically separated from their actions.

Example:

```text
Physical Input
      ↓
Input Handler
      ↓
Action Mapping
      ↓
HID / Firmware Action
```

This allows the physical layout to remain stable while firmware changes the behavior.

Example:

```text
L → LEFT_CLICK

or

L → CTRL

or

L → MEDIA_PLAY_PAUSE
```

---

# 9. Profiles

The control system SHOULD support multiple profiles.

A profile may define:

- Button mappings
- Scroll behavior
- Scroll sensitivity
- Cursor sensitivity
- Recenter behavior
- Motion settings
- Macro assignments

Example:

### Desktop

```text
L → Left Click
M → Middle Click
R → Right Click
Wheel → Scroll
```

### Media

```text
L → Previous Track
M → Play/Pause
R → Next Track
Wheel → Volume
```

### Custom

User-defined mappings.

The final profile system is TBD.

---

# 10. Motion Control

The MPU6050 is expected to provide the primary motion input.

The motion system should translate physical hand movement into cursor movement.

Conceptual pipeline:

```text
MPU6050
   ↓
Sensor Driver
   ↓
Calibration
   ↓
Filtering
   ↓
Motion Processing
   ↓
Cursor Mapping
   ↓
Bluetooth HID
```

Motion behavior should be configurable independently from physical button mappings.

---

# 11. Cursor Controls

The firmware SHOULD eventually support:

- Cursor sensitivity
- X-axis sensitivity
- Y-axis sensitivity
- Motion dead zone
- Smoothing
- Acceleration
- Response curve
- Motion enable/disable
- Re-centering
- Axis inversion
- Sensor orientation configuration

These settings may be profile-specific.

---

# 12. Additional Controls

The initial physical layout intentionally remains minimal.

Future hardware MAY include:

- Additional buttons
- ANO directional buttons
- Side buttons
- Profile button
- Dedicated recenter button
- Status/indicator controls

Additional controls must have a clear ergonomic purpose before being added.

---

# 13. ANO Directional Encoder

If the selected scroll hardware includes directional buttons, they MAY be assigned to:

- Up
- Down
- Left
- Right
- Center

Possible functions include:

- Page navigation
- Horizontal scrolling
- Media controls
- Profile switching
- Recenter
- Keyboard shortcuts
- Custom macros

No final assignment has been made.

---

# 14. Configuration Philosophy

The device should not depend on proprietary desktop software for normal operation.

The firmware should provide the ability to define:

- Button behavior
- Scroll behavior
- Motion sensitivity
- Profiles
- Recenter behavior
- Other supported input mappings

Configuration storage should persist across device restarts where appropriate.

---

# 15. Input Priority

The firmware should ensure that input processing remains responsive.

Priority should generally be:

1. Safety/system state
2. Motion sampling
3. Physical button input
4. Encoder input
5. HID report generation
6. Non-critical background operations

The final scheduling architecture is defined separately in `ARCHITECTURE.md`.

---

# 16. Current Control Requirements

## MUST

- Three primary mouse controls: L, M, R.
- Rotary scrolling.
- Bluetooth HID mouse input.
- Recenter capability.
- Configurable control architecture.
- Natural thumb access to the scroll control.

## SHOULD

- Adjustable scroll sensitivity.
- Configurable button mappings.
- Multiple interaction types.
- Persistent profiles.
- Adjustable cursor behavior.
- Configurable scroll direction.

## MAY

- Macros.
- Media controls.
- Application-specific mappings.
- Horizontal scrolling.
- Additional physical buttons.
- ANO directional controls.

## FUTURE

- Advanced gesture controls.
- More sophisticated profile management.
- External configuration interface.
- Additional sensors.

---

# 17. Open Questions

- Exact scroll encoder model.
- Exact physical button type.
- Exact button dimensions.
- Recenter button location.
- Whether the M button should also support a recenter shortcut.
- Number of profiles.
- Macro support.
- Double-press support.
- Long-press support.
- Hold behavior.
- Final enclosure dimensions.
- Final hand orientation.
- Final physical grip.
- Exact position and orientation of the MPU6050.

---

# 18. Design Principle

The physical control layout should remain simple.

Complexity should primarily live in firmware rather than being added through excessive physical controls.

The goal is a compact device with a small number of intuitive controls that can perform many functions through configurable firmware behavior.
