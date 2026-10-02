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

- The rebuild happened on a `rebuild` branch, merged into `main` by
  fast-forward for 0.2.0 and then deleted; work continues on `main`.
- Same standards as Foundation and MCC: RoModularBuild presets and scripts,
  Bash and PowerShell parity, warnings as errors, sanitizers, clang-tidy,
  installable CMake package, Arduino library layout and Doxygen.
- MIDI concepts live in MIDILAR; music theory stays in MCC; general utilities
  (buffers, callbacks, time, scheduling, flash data) come from Foundation.
- Signal processing is out of scope: it moves to the future DspCore library.
- The Euclidean distribution lives in MIDILAR, next to its only consumer, the
  sequencer; it moves to Foundation only if a non-MIDI consumer appears.
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

Status: complete (`MIDILAR::Devices`): device output wiring, `Router`,
`ChannelFilter`, `ChannelReassign`, `Transpose`, `VelocityCurve`,
`ScaleFilter`, `ChordGenerator` and `HeldNotes`/`HeldValues`.

A device interface on UMP and `Foundation::Functional::Callback`; channel and
group filters and reassignment, velocity curves, transposition, `ScaleFilter`
on `MCC::Scale`, chord generation on `MCC::Chords`, and a router.

## Phase 7 - Clock and sequencing

Status: complete (`ClockGenerator`, `ClockReceiver`, `EuclideanPattern`,
`StepSequencer`; SPEC-CLK-1..3, SPEC-SEQ-1..2). Decision (2026-10-02): the
Euclidean distribution lives in MIDILAR, next to its only consumer, instead
of Foundation. The sequencer counts steps in MIDI clocks; MCC's Rhythm types
(`NoteValue`, `Meter`) can drive step lengths once MCC releases them.

MIDI clock (24 PPQN), start/stop/continue and song position on
`Foundation::Time`; a step sequencer with a Euclidean distribution.

## Phase 8 - System common and MTC

Status: complete. Song select, tune request and song position were already
`Packet` messages (phase 3); MTC adds `TimeCode`, `MtcGenerator` and
`MtcReceiver` (SPEC-MTC-1..3).

Song select, tune request, MTC quarter frames and full frames.

## Phase 9 - Wire formats

Status: complete. Decision (2026-10-02): hardware transports (UART on
Arduino `Serial` or a vendor HAL, USB stacks such as TinyUSB, desktop MIDI
APIs) live in separate libraries, created only when requested. MIDILAR keeps
every hardware-independent conversion between `Packet` and a wire format,
so those libraries stay thin and the conversions are tested without
hardware:

- MIDI 1.0 byte streams (DIN/UART, BLE payloads): `Midi1Parser` and
  `Midi1Encoder` (phase 4).
- USB MIDI 1.0 event packets: `DecodeUsbMidi1()` and `UsbMidi1Encoder`
  (SPEC-USB-1..2).
- USB MIDI 2.0 carries UMP directly and needs no conversion.

## Phase 10 - MIDI-CI

Status: deferred (2026-10-02), with no date. Discovery, profile
configuration and property exchange over UMP; byte layouts to be validated
against the MIDI-CI 1.2 specification when the phase resumes.

## Phase 11 - Quality and merge

Status: complete for 0.2.0 (2026-10-02). Hardware validation on an Arduino
Mega 2560 over USB with Hairless MIDI at 115200 baud:

- `Devices/ClockOut`: stable clock; 119.94 BPM uncalibrated (oscillator about
  500 ppm slow), 120.0 BPM with `-DMIDILAR_EXAMPLE_CLOCK_PPM=-500`; Euclidean
  notes received.
- `Devices/MtcLoopback`: generator, encoder, parser and receiver at 25 fps,
  200 bytes per second, no drift. `Devices/MtcOut` could not be checked
  through Hairless, which treats `0xF1` as a three-byte message.
- `Devices/TransposeThru`: notes transposed and kept on C major, no hanging
  notes.

Not yet run: an Uno with a DIN MIDI shield, and MTC into a DAW. `rebuild`
was merged into `main` by fast-forward and tagged `v0.2.0`.

## Proposed releases

| Version | Scope |
| --- | --- |
| `0.1.0` | Scaffold (phase 0), never released |
| `0.2.0` | Phases 1-9: specification, protocol, UMP, MIDI 1.0 and USB MIDI 1.0 translation, MCC integration, devices, clock and sequencing, MTC |
| `0.3.0` | MIDI-CI (phase 10), when it resumes |
| `1.0.0` | Stable documented API on supported targets |
