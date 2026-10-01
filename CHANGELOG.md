# Changelog

This file records user-visible changes to MIDILAR. Release dates use the
`YYYY-MM-DD` format.

## [Unreleased]

### Changed

- MIDILAR is rebuilt from scratch. The legacy 1.0.0-labelled code (never
  released) was removed; it remains in the Git history before the `rebuild`
  branch.

### Added

- 0.1.0 scaffold: RoModularBuild `v0.4.0`, native, AVR and Arm presets,
  Bash and PowerShell scripts, Foundation `1.4.0` and MCC `0.5.1`
  dependency resolution, installable CMake package with a consumer test,
  Arduino library layout, Doxygen documentation and CI (native, embedded,
  Arduino, documentation, sanitizers and clang-tidy).
- `MIDILAR::Core::Version()`, `MCCVersion()` and `FoundationVersion()`.
- `MIDILAR::Protocol` primitives: `Channel` and `Group` (wire `0-15`),
  `ChannelMask` and `GroupMask`, 7-bit `NoteNumber`, `ControllerNumber` and
  `ProgramNumber`, 16-bit `Velocity`, 32-bit `ControllerValue`,
  `PressureValue` and `PitchBend`, and the MIDI 2.0 min-center-max
  `ScaleUp()`/`ScaleDown()`.
