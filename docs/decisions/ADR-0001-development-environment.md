# ADR-0001: Development Environment

- Status: Accepted
- Date: 2026-09-25

## Context

The project needs a reproducible development workflow that does not depend on Arduino IDE. The user will work primarily in VS Code and wants command-line-friendly tooling that can be used by AI-assisted development workflows.

## Decision

Use **VS Code + PlatformIO** as the primary firmware development environment.

PlatformIO will provide:
- project configuration
- dependency management
- build
- upload/flash
- serial monitor
- test execution where supported

Arduino IDE is not a project requirement.

Claude Code CLI, Codex CLI, Traycer, GSD, and Ralph may be used as development assistants/workflow tools, but they do not replace the firmware build system.

## Consequences

### Positive
- One reproducible project configuration
- Works naturally with VS Code
- CLI-friendly for agent workflows
- Build/flash/monitor commands can be automated

### Negative
- PlatformIO introduces its own project configuration and dependency model
- Hardware-specific upload settings still need verification

## Constraints

Agents must use the repository's PlatformIO configuration as the source of truth and must not invent board, framework, port, or upload settings.
