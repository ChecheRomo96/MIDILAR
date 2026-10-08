# MIDILAR agent instructions

MIDILAR is the MIDI layer of the RoModular ecosystem: protocol data, parsing,
routing, timing and devices. Keep it independently buildable for desktop and
embedded consumers, and preserve the dependency direction
`CPSTL <- Foundation <- (DspCore, MCC) <- MIDILAR`.

MIDILAR was rebuilt from scratch and released as 0.3.0. Work happens on
`main`, following `ACTION_PLAN.md` phase by phase; each phase needs the
user's approval.

## Shared RoModular guidance

Before starting work, look for the shared guidance in
`../RoModularAgents`.

- If it exists and is readable, read `AGENTS.md` and `CONTRACT.md` completely.
- Read `repositories/MIDILAR.md` for the canonical repository adapter.
- Use the relevant skill under `skills/` when the request matches one.
- If the sibling repository is unavailable, continue with the rules in this
  file and report that the shared guidance was not loaded.

Shared guidance does not expand the user's requested scope. Do not modify
Foundation, MCC, RoModular, RoModularBuild, or another sibling repository
unless the user explicitly includes it.

## Repository rules

- Treat `CMakePresets.json`, its included preset files, and the scripts under
  `scripts/` as the supported build interface, with Bash and PowerShell
  parity.
- Initialize the pinned `tools/RoModularBuild` submodule before invoking a
  workflow in a fresh checkout, and treat it as read-only.
- Foundation, DspCore and MCC are resolved by their corresponding
  `cmake/MIDILAR*.cmake` modules; keep their pinned versions in step with the
  Arduino CI job.
- General utilities belong in Foundation, music theory in MCC, and signal
  processing in DspCore; only MIDI concepts belong here.
- Code never throws exceptions. Operations that change a container's
  size may allocate; the library makes no real-time assumptions about the
  caller, and implementers who modify storage in time-critical code are
  expected to reserve the space beforehand. Framework types report allocation
  failure through their result (`bool` or invalid state), never by exceptions
  or undefined behavior (SPEC-RT-1).
- Value types are trivially copyable with compile-time size
  budgets in `src/MIDILAR.cpp`.
- Keep the MIDI-domain specification (`docs/Topics/Specification/MidiDomain.dox`)
  ahead of the code; tests reference its invariants.
- Examples demonstrate public APIs; unit tests live under `tests/MIDILAR/`
  and use GoogleTest through CTest.
- Keep public headers, examples, tests, version metadata, `ACTION_PLAN.md`,
  `CHANGELOG.md` and Doxygen synchronized with public API changes.
- Separate compile/link validation from hardware execution evidence.
- Preserve unrelated work and do not commit, tag, push, publish, or merge
  unless the user explicitly requests it.

## Supported entry points

Use the PowerShell equivalent on Windows.

```text
./scripts/configure.sh <preset> [--fresh] [-- <cmake-options>]
./scripts/build.sh <preset> [--fresh] [--config <configuration>] [--examples-on]
./scripts/test.sh <preset> [--fresh] [--config <configuration>]
./scripts/install.sh <preset>
./scripts/export.sh <preset> [--fresh] [--examples-on]
./scripts/test-package.sh <preset> [--fresh]
./scripts/test-arduino.sh [--fqbn <board>] [--foundation <dir>] [--mcc <dir>]
./scripts/analyze.sh <preset> [--fresh]
./scripts/docs.sh [--fresh]
```
