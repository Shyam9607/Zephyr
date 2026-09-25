# Air Mouse Agent Rules

> **This file is the operating contract for every AI agent working on the Air Mouse project.**

## 1. Purpose

The Air Mouse is a modular ESP32-based wireless air mouse/controller.

The project is developed with AI assistance from multiple tools and agents. Because multiple agents may work on the repository, **consistency and documentation synchronization are mandatory**.

The repository documentation is part of the engineering system, not optional commentary.

---

# 2. Source of Truth

Before making changes, agents MUST read the relevant source-of-truth documents.

At minimum, consult:

- `PRD.md`
- `docs/CONTROLS.md`
- `docs/HARDWARE.md`
- `docs/ARCHITECTURE.md`
- `docs/FIRMWARE.md`
- `docs/TESTING.md`
- relevant files under `docs/decisions/`
- relevant files under `docs/specs/`

If a task concerns hardware, also inspect the applicable datasheets in `datasheets/`.

If the repository structure changes, update the relevant project documentation describing that structure.

### Rule

**Never treat code as the only source of truth.**

If implementation and documentation disagree, stop and determine which is authoritative before proceeding. Do not silently choose one.

---

# 3. Documentation Synchronization Rule

## This is a HARD REQUIREMENT.

**Whenever anything changes, every relevant document MUST be updated in the same change.**

A change is not complete until the documentation describing that change is synchronized.

Examples:

### Hardware change
If a GPIO, sensor, component, interface, voltage, connector, or physical placement changes:

Update every relevant document, potentially including:

- `docs/HARDWARE.md`
- `docs/CONTROLS.md`
- `docs/ARCHITECTURE.md`
- `docs/FIRMWARE.md`
- `docs/TESTING.md`
- relevant ADR
- PRD/spec documents if the product requirement changed

### Control change
If a button, encoder, scroll behavior, gesture, mapping, profile, or interaction changes:

Update every relevant document, potentially including:

- `docs/CONTROLS.md`
- `docs/HARDWARE.md` if physical hardware is affected
- `docs/ARCHITECTURE.md`
- `docs/FIRMWARE.md`
- `docs/TESTING.md`
- PRD/specs if the product behavior changed
- relevant ADR

### Firmware architecture change
Update:

- `docs/ARCHITECTURE.md`
- `docs/FIRMWARE.md`
- `docs/TESTING.md`
- relevant ADR
- PRD/specs if externally visible behavior changes

### Configuration/profile change
Update:

- `docs/CONTROLS.md`
- `docs/FIRMWARE.md`
- `docs/TESTING.md`
- relevant architecture/spec documents
- relevant ADR

### Testing change
Update:

- `docs/TESTING.md`
- relevant feature documentation
- relevant ADR if the testing strategy itself changed

### Repository structure change
Update:

- `AGENTRULES.md`
- relevant development documentation
- relevant architecture/firmware documentation

## Documentation Impact Check

Before finishing any task, the agent MUST ask:

> **"What documents became stale because of this change?"**

Then update all applicable documents.

Do not wait for the user to request documentation updates.

---

# 4. No Silent Decisions

Agents MUST NOT silently convert an assumption into a project decision.

If a new technical decision is required:

1. Identify the decision.
2. Check existing ADRs.
3. If the decision is already covered, follow the existing ADR.
4. If the decision conflicts with an existing ADR, flag the conflict.
5. If a genuinely new architectural/product decision is made, create or update an ADR.
6. Update all affected documentation.

Use explicit statuses in ADRs:

- `Proposed`
- `Accepted`
- `Superseded`
- `Rejected`

Never pretend a proposed decision is final.

---

# 5. Hardware Rules

Hardware assumptions are especially dangerous.

Agents MUST:

- Never invent GPIO assignments.
- Never assume a pin is safe without checking the relevant board/module documentation.
- Never assume the exact ESP32 DevKit revision.
- Verify bootstrapping/strapping pins before assigning peripherals.
- Verify voltage and logic-level requirements.
- Verify I2C addresses.
- Verify sensor orientation.
- Verify interrupt requirements.
- Verify power requirements.
- Record hardware changes in documentation.
- Record important hardware decisions in ADRs.

### Battery safety

Do not connect a Li-ion/LiPo battery directly to an ESP32 3.3 V rail unless the complete power architecture explicitly supports it.

Battery, charging, regulation, protection, boost/buck conversion, and switching must be treated as separate engineering decisions.

---

# 6. Prototype Before Final Hardware

The current project is prototype-first.

Do not prematurely optimize for:

- custom PCB
- final enclosure
- final dimensions
- production BOM
- production power architecture

unless the current task explicitly requires it.

Prototype assumptions may change.

When prototype findings invalidate an existing assumption:

1. document the finding,
2. update the relevant docs,
3. update/supersede the relevant ADR,
4. update implementation and tests.

---

# 7. Firmware Architecture Rules

Firmware should follow the modular architecture defined in `docs/ARCHITECTURE.md`.

Expected conceptual boundaries include:

```text
Hardware Drivers
       ↓
IMU / Controls
       ↓
Motion Processing / Input Processing
       ↓
Action Mapping
       ↓
HID
       ↓
Host
```

Supporting systems include:

- profiles/configuration
- persistent storage
- diagnostics
- power management

### `main.cpp`

`main.cpp` should remain small.

Do not dump feature logic, sensor processing, button logic, or HID mapping into `main.cpp`.

Initialization and high-level orchestration belong there. Feature implementation belongs in the appropriate module.

---

# 8. Hardware-Specific Logic

Hardware-specific constants and behavior should be isolated.

Examples:

- GPIO numbers
- I2C address
- sensor orientation
- timing constants
- electrical assumptions
- board-specific workarounds

Do not scatter these throughout application code.

If hardware-specific values become important enough to affect architecture or documentation, record them in the relevant hardware documentation and/or ADR.

---

# 9. Input and Action Separation

Physical controls and actions must remain separate concepts.

For example:

```text
Physical Input
    ↓
Input Event
    ↓
Action Mapping
    ↓
Action
    ↓
HID / System Output
```

Do not hard-code:

```text
GPIO X = always left click
```

inside unrelated hardware or HID code when the architecture supports remapping.

The goal is to allow future:

- profiles
- remapping
- hold actions
- long press
- double press
- macros
- media actions
- keyboard actions
- application-specific mappings

without rewriting the physical input layer.

---

# 10. Motion Pipeline Rules

Motion processing should follow the documented pipeline:

```text
MPU6050
  ↓
Calibration
  ↓
Filtering
  ↓
Motion Processing
  ↓
Cursor Mapping
  ↓
HID
```

The motion system should keep configuration separate from raw sensor acquisition.

Potential configurable parameters include:

- sensitivity
- X/Y sensitivity
- dead zone
- smoothing
- acceleration
- response curve
- axis inversion
- motion enable/disable
- recentering

Any change to this pipeline or its externally visible behavior requires documentation synchronization.

---

# 11. Recenter Is an Event

Recenter functionality should be treated as a logical action/event rather than being permanently tied to one physical button.

This allows recentering to be assigned to:

- a dedicated control
- a button hold
- a combination
- another future input mechanism

Physical assignment and logical behavior should remain separate.

---

# 12. Configuration and Profiles

Configuration stored on the device must be treated as versioned data.

Agents must consider:

- schema version
- default values
- validation
- corruption recovery
- migration when necessary
- backward compatibility where applicable

Invalid configuration must fall back safely to known defaults.

If a configuration field is added, removed, renamed, or changes meaning:

1. update firmware,
2. update configuration documentation,
3. update tests,
4. update migration/default logic if required,
5. update the relevant ADR/spec.

---

# 13. Bluetooth HID Rules

Bluetooth HID is the primary wireless host interface.

At minimum, the mouse behavior must support:

- cursor movement
- left click
- middle click
- right click
- scroll

The HID layer must not own physical button interpretation.

Host compatibility must be tested rather than assumed.

Do not claim Bluetooth compatibility or successful pairing without appropriate testing evidence.

---

# 14. Testing Rules

Tests must reflect actual project behavior.

Agents MUST:

- run relevant tests after changes,
- add tests for new logic where practical,
- preserve existing tests unless intentionally changing requirements,
- never modify tests solely to make an implementation pass,
- distinguish unit tests from hardware tests,
- identify hardware-dependent tests,
- never claim a physical test was performed when it was not.

### Required honesty

If an agent cannot physically test the ESP32, it must say so.

For example:

> Build and static/unit validation completed. Physical MPU6050 and Bluetooth HID validation requires hardware execution.

Do not manufacture hardware-test results.

---

# 15. Build and Tooling Rules

The project uses VS Code + PlatformIO.

Agents should use the repository's configured build system.

Do not introduce Arduino IDE as a project requirement.

Do not replace PlatformIO configuration merely because another tool is more familiar.

If the framework changes between Arduino and ESP-IDF, treat that as an architectural decision and document it with an ADR.

---

# 16. Dependencies

Before adding a dependency:

1. Determine whether an existing project dependency already provides the required functionality.
2. Check whether the dependency is compatible with the selected framework and board.
3. Consider maintenance, footprint, licensing, and reliability.
4. Update dependency configuration.
5. Update relevant documentation.
6. Update tests if behavior changes.

Do not add libraries casually.

---

# 17. Code Quality

Prefer:

- small focused modules
- explicit interfaces
- descriptive names
- deterministic behavior
- bounded memory usage
- non-blocking runtime behavior where practical
- short interrupt handlers
- clear error handling
- testable logic

Avoid:

- giant functions
- hidden global state
- unnecessary dynamic allocation
- blocking delays in time-sensitive paths
- duplicated hardware logic
- magic numbers
- undocumented assumptions

---

# 18. Interrupt Rules

Interrupt handlers must remain short.

An interrupt handler should generally:

- capture the event,
- update minimal state,
- signal work to the main/runtime context.

Do not perform expensive processing, logging floods, complex HID operations, or blocking operations directly inside an interrupt handler unless the architecture explicitly requires and documents it.

---

# 19. Logging and Diagnostics

Use the project's defined logging levels:

- ERROR
- WARN
- INFO
- DEBUG
- TRACE

Diagnostic output must not destroy runtime responsiveness.

Temporary debug instrumentation should either be:

- removed after the experiment, or
- converted into intentional diagnostic functionality.

Do not leave unexplained debug prints in production paths.

---

# 20. Change Classification

Before modifying code, classify the change.

### Type A: Implementation-only

Example:

- refactor internal code
- optimize an algorithm without changing behavior

Documentation may not need content changes, but the agent must still perform the documentation impact check.

### Type B: Behavioral

Example:

- sensitivity behavior changes
- button mapping changes
- scroll direction changes
- recenter behavior changes

Update all behavior-related documentation and tests.

### Type C: Hardware

Example:

- GPIO change
- sensor change
- component change
- power change

Update hardware, architecture, firmware, testing, and ADR documentation as applicable.

### Type D: Architectural

Example:

- changing framework
- changing HID architecture
- changing storage architecture
- changing module boundaries

Requires an ADR and synchronization of all affected documentation.

### Type E: Product requirement

Example:

- adding/removing a required feature
- changing the definition of done
- changing the intended user experience

Update PRD and all affected downstream documents.

---

# 21. Definition of Done for AI Agents

A task is NOT complete merely because the code compiles.

Before reporting completion, the agent must verify:

- [ ] Requirements understood
- [ ] Relevant documentation read
- [ ] Existing ADRs checked
- [ ] Implementation completed
- [ ] Relevant tests added/updated
- [ ] Relevant tests executed
- [ ] Hardware-dependent validation identified
- [ ] Documentation impact checked
- [ ] All relevant docs synchronized
- [ ] ADR created/updated if a decision changed
- [ ] No undocumented hardware assumptions introduced
- [ ] No stale TODO caused by the change
- [ ] Final status accurately reports what was and was not tested

---

# 22. Multi-Agent Workflow

Multiple agents may work on the same repository.

Therefore:

### Never assume you are the only agent.

Before changing architecture or shared interfaces:

1. Read current documentation.
2. Check recent changes in the repository.
3. Check relevant ADRs.
4. Preserve established interfaces unless the task explicitly changes them.

Agents must avoid rewriting unrelated work.

Keep changes scoped to the requested task.

---

# 23. Agent Roles

Different tools may have different responsibilities.

### Project Manager / Planner

Traycer or GSD may:

- break work into tasks
- identify dependencies
- track requirements
- propose implementation plans

They must respect the repository documentation and must not invent requirements.

### Primary Implementation Agent

Claude Code may:

- implement features
- modify firmware
- add tests
- update documentation
- perform local validation

### Review / Verification Agent

Codex may:

- inspect implementation
- identify bugs
- review architecture
- check tests
- identify documentation drift
- suggest corrections

### Iterative Loop

Ralph may:

- repeatedly execute scoped tasks
- run tests
- inspect failures
- apply bounded fixes

Ralph must not continuously mutate architecture without explicit documented decisions.

---

# 24. Agent Handoff Contract

When handing work to another agent, leave enough information for the next agent to continue safely.

A handoff should identify:

- what changed
- why it changed
- files affected
- tests run
- tests not run
- known limitations
- unresolved questions
- documentation updated
- ADRs created/modified

The repository should remain understandable without relying on private agent conversation history.

---

# 25. Documentation Drift Is a Bug

Documentation drift is considered an engineering defect.

If code says one thing and the documented architecture says another, the repository is in an inconsistent state.

Agents should actively detect and correct this.

The desired invariant is:

```text
Requirements
     ↓
Architecture
     ↓
Implementation
     ↓
Tests
     ↓
Documentation
```

These layers must remain mutually consistent.

---

# 26. Change Propagation Rule

When a decision changes, propagate it through the project.

Use this mental model:

```text
CHANGE
  │
  ├── What requirement changed?
  ├── What hardware changed?
  ├── What architecture changed?
  ├── What firmware changed?
  ├── What controls changed?
  ├── What tests changed?
  ├── What ADR changed?
  └── What other documents are now stale?
```

**Never stop after editing the first obvious file.**

Trace the change through every affected layer.

---

# 27. Final Rule

> **If you change it, document it.**
>
> **If you decide it, record it.**
>
> **If you test it, report what was actually tested.**
>
> **If you do not know, verify it.**
>
> **If you cannot verify it, mark it as unknown instead of inventing an answer.**

This rule applies to every human and AI contributor to the project.
