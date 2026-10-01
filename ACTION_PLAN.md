# MIDILAR Reconstruction Action Plan

## Objective

Rebuild MIDILAR from scratch as a portable C++17 MIDI library for desktop and
embedded targets. MIDILAR owns MIDI protocol data, parsing, routing, timing and
devices. It consumes Foundation for general utilities and MCC for music
theory, and it never allocates memory or throws exceptions in real-time paths.

The dependency direction is:

```text
Foundation <- MCC <- MIDILAR
```

The legacy MIDILAR code (labelled 1.0.0, never released, no consumers) was
removed when this reconstruction started. It remains in the Git history before
the `rebuild` branch and in the read-only audit, as design input only.

## Architectural decisions

- Work happens on the `rebuild` branch until MIDILAR is stable; then it merges
  into `main`.
- Same standards as Foundation and MCC: RoModularBuild presets and scripts,
  Bash and PowerShell parity, warnings as errors, sanitizers, clang-tidy,
  installable CMake package, Arduino library layout and Doxygen.
- MIDI concepts live in MIDILAR; music theory stays in MCC; general utilities
  (buffers, callbacks, time, scheduling, flash data) come from Foundation.
- Signal processing is out of scope: it moves to the future DSPCore library.
- The generic Euclidean distribution goes to Foundation; MIDILAR's sequencer
  consumes it.
- Value types are trivially copyable, `constexpr` where practical, with one
  canonical invalid value and compile-time size budgets for every target.
- Variable-length data (SysEx) uses caller-provided buffers.

## Phase 0 - Baseline

Status: complete

- [x] Remove the legacy code and start the `rebuild` branch.
- [x] Pin RoModularBuild `v0.4.0`; native, AVR and Arm presets; scripts.
- [x] Resolve Foundation `1.4.0` and MCC `0.5.1` (package or sources).
- [x] Installable `MIDILAR::MIDILAR` package with a consumer test.
- [x] Core module with MIDILAR, MCC and Foundation versions.
- [x] CI: native, embedded, Arduino, documentation, sanitizers, clang-tidy.

## Phase 1 - MIDI-domain specification

Write `docs/Topics/Specification/MidiDomain.dox` before any MIDI type:

- [ ] Status and data bytes, channel numbering (wire 0-15, user 1-16).
- [ ] Note numbers 0-127 and their relation to `MCC::ChromaticIndex`
  (MIDI 60 = C4, matching MCC).
- [ ] Velocity, Note On with velocity 0, and Note Off policy.
- [ ] Controllers, 14-bit values (pitch bend, NRPN/RPN) and program changes.
- [ ] Running status, real-time messages interleaved inside other messages,
  and SysEx framing.
- [ ] Error and invalid-value policy without exceptions.

## Phase 2 - Protocol primitives

`Channel`, `NoteNumber`, `Velocity`, `ControllerNumber`, `ProgramNumber`,
`PitchBend` and the status-byte families as `constexpr` value types with
invalid states and exhaustive tests.

## Phase 3 - Messages

A fixed-size `Message` (status plus up to two data bytes) with typed
constructors and queries per message kind, and SysEx as a view over a caller
buffer.

## Phase 4 - Parser

Byte stream to messages: running status, real-time interleaving, SysEx
streaming into a caller buffer, recovery from malformed input, no allocation.

## Phase 5 - MCC integration

Checked conversions between `NoteNumber` and `MCC::Pitch`/`ChromaticIndex`,
rejection outside 0-127, and spelling of incoming notes with `MCC::Key`.
Closes MCC phase 10 and MIG-008.

## Phase 6 - Devices and routing

A device interface on `Foundation::Functional::Callback`; channel filter and
reassignment, velocity curves, transposition, `ScaleFilter` on `MCC::Scale`,
chord generation on `MCC::Chords`, and a router.

## Phase 7 - Clock and sequencing

MIDI clock (24 PPQN), start/stop/continue and song position on
`Foundation::Time`/`Scheduling`; a sequencer using Foundation's Euclidean
distribution and MCC's rhythm types (designed together with MCC phase 9).

## Phase 8 - System common and MTC

Song select, tune request, MTC quarter frames and full frames.

## Phase 9 - Transports

UART (Arduino `Serial`), USB MIDI and desktop adapters, each optional.

## Phase 10 - Quality and merge

Hardware validation of the transports, documentation without warnings,
release workflow and merge of `rebuild` into `main`.

## Proposed releases

| Version | Scope |
| --- | --- |
| `0.1.0` | Scaffold (phase 0) |
| `0.2.0` | Specification and protocol primitives |
| `0.3.0` | Messages and parser |
| `0.4.0` | MCC integration, devices and routing |
| `0.5.0` | Clock and sequencing |
| `0.6.0` | System common and MTC |
| `0.7.0` | Transports |
| `1.0.0` | Stable documented API on supported targets |
