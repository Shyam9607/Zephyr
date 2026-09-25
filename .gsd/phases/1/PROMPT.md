# Zephyr Phase 1 Ralph Execution Prompt

You are executing Phase 1 of the Zephyr ESP32 air-mouse project inside an existing GSD-managed repository.

## Operating mode

Ralph provides the iteration loop and fresh context. Work on **exactly one task from `RALPH-TASKS.md` per iteration**.

Do not invoke `/execute` recursively. The GSD Phase 1 plan files are the authoritative engineering instructions that you must read and execute directly.

At the beginning of every iteration:

1. Read `RALPH-TASKS.md`.
2. Read `RALPH-PROGRESS.md`.
3. Read the applicable GSD Phase 1 plan.
4. Read `.gsd/SPEC.md`, `.gsd/DECISIONS.md`, `.gsd/REQUIREMENTS.md`, `.gsd/ROADMAP.md`, `.gsd/STATE.md`.
5. Read `AGENTRULES.md` and `PROJECT_RULES.md`.
6. Inspect the current repository state and recent commits.
7. Identify the first incomplete Ralph task whose prerequisites are satisfied.

## Hard constraints

- Follow the existing source-of-truth hierarchy.
- Do not invent hardware facts.
- Do not assign GPIOs, MPU6050 orientation, I2C electrical details, or exact physical controls when they are still TBD.
- Do not claim physical hardware validation without physical hardware.
- Host builds, unit tests, static inspection, and mock tests are not physical hardware verification.
- Do not replace measured benchmark evidence with generic web numbers.
- Do not predetermine the Arduino vs ESP-IDF decision.
- Do not weaken acceptance criteria to make a task pass.
- Do not silently rewrite architecture.
- Any architecture decision must be documented through the repository's ADR process.
- Keep production firmware separate from scratch benchmark spikes.
- Preserve framework independence in domain logic through interfaces/HAL boundaries as specified by the project.
- Update every relevant document in the same change when behavior, architecture, decisions, requirements, or execution state changes.
- Keep commits atomic and scoped to the current task.
- Do not push to the remote unless explicitly instructed.
- Never delete or rewrite historical progress entries.

## Verification discipline

Before declaring a task complete:

1. Run the relevant build/test/inspection commands.
2. Inspect the resulting output.
3. Confirm the evidence actually supports the claim.
4. Update the progress file with what changed and the evidence.
5. Commit the scoped work if the repository workflow requires a commit.
6. Re-check `git status` and relevant diffs.

If a task is blocked by missing hardware, missing credentials, unavailable tooling, or genuinely missing evidence:

- do not fabricate a result;
- document the blocker and exact evidence;
- stop that task rather than guessing.

If an architectural decision or source-of-truth conflict is discovered:

- stop before making a speculative change;
- document the conflict in progress;
- request human review rather than repeatedly mutating the architecture.

## Recovery rule

If the same failure is encountered three times without new evidence, stop trying variations and document the blocker for human review.

## Completion

The loop may terminate only when all Ralph tasks are complete, Phase 1 verification passes, the repository is internally consistent, and the required GSD state/documentation updates are present.

The Ralph extension will supply its unique completion marker. Append that marker to the progress file exactly as instructed by the extension when the entire task list is genuinely complete.
