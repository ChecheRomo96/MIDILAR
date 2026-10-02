# Changelog

This file records user-visible changes to MIDILAR. Release dates use the
`YYYY-MM-DD` format.

## [Unreleased]

## [0.2.0] - 2026-10-02

First release of the rebuilt MIDILAR: phases 1-9 of `ACTION_PLAN.md`.
Validated on an Arduino Mega 2560 over USB with a serial-to-MIDI bridge
(`Devices/ClockOut`, `Devices/MtcLoopback`, `Devices/TransposeThru`).

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
- `MIDILAR::Protocol::Packet`: UMP packets with typed constructors and
  queries for utility, system common and real-time, MIDI 1.0 and MIDI 2.0
  channel voice (including note attributes and banked program change) and
  7-bit System Exclusive packets.
- MIDI 1.0 translation: `Midi1Parser` (byte stream to UMP with running
  status, real-time interleaving and malformed-input recovery),
  `Midi1Encoder` (UMP to byte stream with optional running status, MIDI 2.0
  channel voice scaled down) and `SysExAssembler` (payloads into a caller
  buffer with truncation). `MIDILAR_PARSER_DIAGNOSTICS` adds parser error
  counters.
- MCC integration: `ToChromaticIndex()`, `ToNoteNumber()` (from a chromatic
  index or any spelling of an `MCC::Pitch`, invalid outside 0-127) and
  `ToPitch()` (spelled in an `MCC::Key`, C major by default).
- `MIDILAR::Devices`: devices chained through `Foundation::Functional::Callback`
  outputs (`ChannelFilter`, `ChannelReassign`, `Transpose`, `VelocityCurve`,
  `ScaleFilter` on `MCC::Scale`) and a fan-out `Router`. `Packet` gains
  `HasNote()`, `WithGroup()`, `WithChannel()` and `WithNote()`.
- `HeldNotes`: `Transpose` and `ScaleFilter` release each held note on the
  note it started on, even after their mapping changes.
- MIDILAR requires MCC `0.5.2` or a newer `0.x` release.
- MIDI clock: `ClockGenerator` sends timing clocks at a tempo from a
  `Foundation::Time::Clock` without drift, with Start, Stop, Continue and
  Song Position; `ClockReceiver` follows transport, position and tempo.
  New `Devices/ClockOut` example.
- `EuclideanPattern()` and `StepSequencer`: Euclidean rhythms of up to 64
  steps played on MIDI clock, with gate, step length and song position.
- MIDI Time Code: `TimeCode` (24, 25, 29.97 drop-frame and 30 fps,
  quarter-frame encoding), `MtcGenerator` (quarter frames without drift and
  full frames) and `MtcReceiver`. New `Devices/MtcOut` example.
- USB MIDI 1.0: `DecodeUsbMidi1()` turns 4-byte event packets into UMP
  (cable = group) and `UsbMidi1Encoder` turns UMP into events, with System
  Exclusive kept per cable. Hardware transports belong to separate
  libraries.
- `ChordGenerator`: plays an `MCC::ChordPattern` on every note and releases
  each held note with the chord it started with.
