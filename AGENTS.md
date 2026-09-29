# MIDILAR agent instructions

MIDILAR is the MIDI, transport, device, and real-time music-technology layer of
the RoModular ecosystem. The current repository is legacy code pending a
deliberate reconstruction, so preserve evidence and avoid broad modernization
unless the user explicitly scopes it.

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

- Preserve the dependency direction `Foundation <- MCC <- MIDILAR`. General
  utilities belong in Foundation, music-theory concepts belong in MCC, and
  MIDI protocol, transport, routing, devices, and real-time processing belong
  here.
- Treat the current source, examples, tests, CMake files, and documentation as
  migration evidence. Do not delete or mechanically rewrite legacy material
  before its behavior and replacement destination are understood.
- The repository has not adopted RoModularBuild yet. Do not bypass its current
  CMake interface or introduce a partial shared-build migration without an
  explicit migration task and validation plan.
- Preserve the current C++17 requirement until a compatibility decision is
  documented and tested for every supported consumer.
- Keep real-time and embedded paths allocation-conscious, exception-free where
  required by the target, and free from mandatory full-STL assumptions.
- Keep Arduino examples and desktop examples aligned around shared public API
  demonstrations. Examples are not unit tests; unit tests remain under
  `tests/`.
- Verify claims in `readme.md`, generated documentation, package metadata, and
  build files against the implementation. This legacy repository may contain
  stale or contradictory documentation.
- `CMakeCache.txt` is currently tracked by the repository. Treat its removal as
  a deliberate cleanup change, not as incidental generated-file deletion.
- Preserve unrelated work and do not commit, tag, push, publish, or merge
  unless the user explicitly requests it.

## Current entry points

Until the repository is reconstructed, use its checked-in CMake configuration:

```text
cmake -B build -S . -DMIDILAR_TESTING=ON -DMIDILAR_EXAMPLES=ON
cmake --build build
ctest --test-dir build
```

The presets in `CMakePresets.json` are legacy convenience configurations, not
yet a RoModularBuild compatibility contract. Inspect their cache variables and
binary directories before using or changing them.

Run narrowly scoped checks first. State clearly which desktop compiler,
Arduino toolchain, cross-compiler, and hardware checks were not available on
the current host.
