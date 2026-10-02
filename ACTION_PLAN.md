# MIDILAR Reconstruction Action Plan

## Objective

Rebuild MIDILAR from scratch as a portable C++17 MIDI library for desktop and
embedded targets. MIDILAR owns MIDI 1.0 and MIDI 2.0 (UMP) protocol data,
parsing, routing, timing and devices. It consumes Foundation for general utilities and MCC for music
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
- Signal processing is out of scope: it moves to the future DspCore library.
- The generic Euclidean distribution goes to Foundation; MIDILAR's sequencer
  consumes it.
- Value types are trivially copyable, `constexpr` where practical, with one
  canonical invalid value and compile-time size budgets for every target.
- Variable-length data (SysEx) uses caller-provided buffers.
- MIDI 2.0 Universal MIDI Packets are the internal model; MIDI 1.0 byte
  streams are translated at the edges.

## Phase 0 - Baseline

Status: complete

- [x] Remove the legacy code and start the `rebuild` branch.
- [x] Pin RoModularBuild `v0.4.0`; native, AVR and Arm presets; scripts.
- [x] Resolve Foundation `1.4.0` and MCC `0.5.1` (package or sources).
- [x] Installable `MIDILAR::MIDILAR` package with a consumer test.
- [x] Core module with MIDILAR, MCC and Foundation versions.
- [x] CI: native, embedded, Arduino, documentation, sanitizers, clang-tidy.

## Phase 1 - MIDI-domain specification

Status: complete (`docs/Topics/Specification/MidiDomain.dox`)

- [x] Internal model: MIDI 2.0 UMP; MIDI 1.0 byte streams translated at the
  edges.
- [x] Channels and groups store `0-15` with explicit `FromWire`/`FromNumber`;
  Omni and every-group are `ChannelMask`/`GroupMask` sets.
- [x] Note numbers equal `MCC::ChromaticIndex` (MIDI 60 = C4).
- [x] MIDI 1.0 Note On with velocity 0 becomes Note Off.
- [x] Running status, real-time interleaving (also inside System Exclusive),
  error policy without exceptions.
- [x] System Exclusive into caller buffers with a truncation flag.
- [x] Error counters only behind `MIDILAR_PARSER_DIAGNOSTICS`.

Decisions:

- UMP as the single internal model keeps devices, routing and timing written
  once and makes MIDI 2.0 the natural growth path; MIDI 1.0 is a translation
  layer, not a second model.
- Values are stored at MIDI 2.0 resolution and scaled with the specification's
  rules, so MIDI 1.0 values survive a round trip.

## Phase 2 - Protocol primitives

Status: complete (`MIDILAR::Protocol`)

`Channel`, `Group`, `ChannelMask`, `GroupMask`, `NoteNumber`, `ControllerNumber`, `ProgramNumber`,
`Velocity` (16-bit) and 32-bit controller, pressure and pitch-bend values as
`constexpr` value types with invalid states, MIDI 1.0 views and the
min-center-max scaling, with exhaustive tests.

## Phase 3 - UMP packets and messages

Status: complete (`MIDILAR::Protocol::Packet`). MIDI 2.0 per-note,
registered/assignable controller messages, 8-bit System Exclusive, flex data
and stream messages are classified but get typed constructors when a later
phase needs them.

The UMP packet types (32/64/96/128-bit), message-type and status
classification, and typed constructors and queries for MIDI 1.0 and MIDI 2.0
channel voice, system common/real-time, utility and System Exclusive packets.

## Phase 4 - MIDI 1.0 translation

Status: complete (`Midi1Parser`, `Midi1Encoder`, `SysExAssembler`)

Byte stream to UMP (running status, real-time interleaving, System Exclusive
into caller buffers, recovery from malformed input) and UMP to byte stream
(optional running status), with round-trip property tests and no allocation.

## Phase 5 - MCC integration

Status: complete in MIDILAR (`ToChromaticIndex`, `ToNoteNumber`, `ToPitch`);
closing MCC phase 10 and MIG-008 is a change in the MCC repository.

Checked conversions between `NoteNumber` and `MCC::Pitch`/`ChromaticIndex`,
rejection outside 0-127, and spelling of incoming notes with `MCC::Key`.
Closes MCC phase 10 and MIG-008.

## Phase 6 - Devices and routing

A device interface on UMP and `Foundation::Functional::Callback`; channel and
group filters and reassignment, velocity curves, transposition, `ScaleFilter`
on `MCC::Scale`, chord generation on `MCC::Chords`, and a router.

## Phase 7 - Clock and sequencing

MIDI clock (24 PPQN), start/stop/continue and song position on
`Foundation::Time`/`Scheduling`; a sequencer using Foundation's Euclidean
distribution and MCC's rhythm types (designed together with MCC phase 9).

## Phase 8 - System common and MTC

Song select, tune request, MTC quarter frames and full frames.

## Phase 9 - Transports

UART (Arduino `Serial`), USB MIDI 1.0/2.0 and desktop adapters, each optional.

## Phase 10 - MIDI-CI

Discovery, profile configuration and property exchange over UMP.

## Phase 11 - Quality and merge

Hardware validation of the transports, release workflow and merge of
`rebuild` into `main`.

## Proposed releases

| Version | Scope |
| --- | --- |
| `0.1.0` | Scaffold (phase 0) |
| `0.2.0` | Specification and protocol primitives |
| `0.3.0` | UMP packets, messages and MIDI 1.0 translation |
| `0.4.0` | MCC integration, devices and routing |
| `0.5.0` | Clock and sequencing |
| `0.6.0` | System common and MTC |
| `0.7.0` | Transports |
| `0.8.0` | MIDI-CI |
| `1.0.0` | Stable documented API on supported targets |
