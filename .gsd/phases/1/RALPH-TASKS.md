# Zephyr Phase 1 Ralph Tasks

## Purpose

Execute Zephyr Phase 1 using the existing GSD Phase 1 plans as the authoritative engineering plan.

Ralph is the execution loop. Do **not** invoke GSD `/execute` recursively. Instead, read the applicable GSD plan and execute its tasks directly while preserving all GSD constraints, dependencies, evidence requirements, and documentation synchronization rules.

The authoritative sources remain:

1. `docs/PRD.md`
2. `docs/` engineering specifications
3. `docs/decisions/`
4. `.gsd/SPEC.md`
5. `.gsd/ROADMAP.md`
6. `.gsd/STATE.md`
7. `.gsd/DECISIONS.md`
8. `.gsd/REQUIREMENTS.md`
9. `AGENTRULES.md`
10. `PROJECT_RULES.md`
11. The current Phase 1 GSD plan files

Ralph must never treat this file as a replacement for those sources.

## Task 1: Execute GSD Plan 1.1, part A

Read the Phase 1 Plan 1.1 file and establish the minimal comparable PlatformIO spike environments for Arduino + NimBLE and ESP-IDF + NimBLE.

Requirements:
- Use the exact board/framework/library constraints in the GSD plan.
- Keep both spikes minimal and functionally comparable.
- Do not implement Zephyr production firmware.
- Do not invent hardware measurements or physical validation.
- Record commands, environment/version information, and raw evidence as required by Plan 1.1.
- Preserve the repository's protected TBDs.

Completion evidence:
- Both spike configurations exist and are buildable or any blocker is documented with evidence.
- The ESP-IDF spike must be functionally comparable to the Arduino spike for the required minimal BLE HID mouse capability, not merely BLE initialization.
- No production architecture is changed as part of this task.

## Task 2: Execute GSD Plan 1.1, part B

Complete the Plan 1.1 benchmark procedure for both framework spikes.

Measure and record only locally observed data, including as applicable:
- clean build time
- incremental build time
- static RAM usage
- flash usage
- dependency/library footprint
- reproducibility/version information
- exact commands and relevant build output

Use equivalent procedures for Arduino and ESP-IDF.

Write/update `scratch/spikes/BENCHMARK_REPORT.md` according to the GSD plan.

Do not substitute generic internet numbers for local measurements.

Completion evidence:
- The benchmark report contains methodology, environment, commands, raw/traceable evidence, and limitations.
- Both frameworks were evaluated using comparable procedures.
- Any missing measurement is explicitly marked as unavailable rather than guessed.

## Task 3: Execute GSD Plan 1.2

Only begin after Tasks 1 and 2 are genuinely complete.

Read and follow the complete GSD Plan 1.2.

Evaluate Arduino vs ESP-IDF against the six required criteria in the specified priority order:
1. BLE HID device capability and implementation quality
2. Resource usage / RAM and flash headroom
3. Long-term maintainability / architectural fit
4. Development velocity / library ecosystem
5. Build/configuration complexity / reproducibility
6. Host-testability / framework independence

Use actual Phase 1 benchmark evidence for empirical claims.

Create/update:
- `docs/decisions/ADR-0009-framework-selection.md`

The ADR must:
- document the criteria and methodology
- distinguish measured facts from qualitative engineering judgment
- include limitations
- document rejected alternatives
- record the final framework decision and rationale
- avoid inventing measurements
- preserve the HAL/domain-logic separation requirement

Do not choose a framework before the evidence is reviewed.

Completion evidence:
- ADR-0009 is complete and internally consistent with the measured benchmark report.
- The selected framework is explicitly recorded.
- No contradictory framework statements remain in the relevant decision/spec files.

## Task 4: Execute GSD Plan 1.3

Only begin after Task 3 is complete.

Read and follow the complete GSD Plan 1.3.

Synchronize all affected sources of truth, including as applicable:
- `docs/FIRMWARE.md`
- `.gsd/DECISIONS.md`
- `.gsd/REQUIREMENTS.md`
- `.gsd/ROADMAP.md`
- `.gsd/STATE.md`

Document the selected framework and the framework-independent HAL architecture.

Mark the relevant Phase 1 requirement complete only when its evidence exists.

Update Phase 1 status and the next roadmap phase only according to the GSD plan.

Completion evidence:
- Documentation is synchronized.
- GSD state reflects the actual completed work.
- No stale contradictory framework/TBD statements remain.
- Verification has been performed.

## Task 5: Phase 1 final verification

Perform a final read-only consistency and verification pass over all Phase 1 outputs.

Check:
- `docs/PRD.md`
- relevant `docs/*.md`
- `docs/decisions/ADR-0009-framework-selection.md`
- `.gsd/SPEC.md`
- `.gsd/DECISIONS.md`
- `.gsd/REQUIREMENTS.md`
- `.gsd/ROADMAP.md`
- `.gsd/STATE.md`
- `AGENTRULES.md`
- `PROJECT_RULES.md`
- benchmark evidence

Verify that:
- no physical hardware test is claimed without hardware
- no GPIO/pinout/orientation/electrical TBD was silently invented
- benchmark numbers are locally evidenced
- the framework decision matches ADR-0009
- docs and GSD state agree
- commits are present and cleanly scoped

If a contradiction or verification failure is found, fix it only within the Phase 1 scope and re-verify.

When every task above is complete and verified, append the Ralph completion marker requested by the extension to the progress file.

## Global completion rule

Phase 1 is **not complete** merely because code builds.

Phase 1 is complete only when the GSD plans, empirical evidence, ADR-0009, documentation synchronization, GSD state, and final verification all agree.
