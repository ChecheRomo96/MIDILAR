# MIDILAR

**Documentation:** [API and build guide](https://checheromo96.github.io/MIDILAR/) · [API class index](https://checheromo96.github.io/MIDILAR/annotated.html)

MIDILAR is the MIDI layer of the RoModular ecosystem: MIDI 1.0 and MIDI 2.0
protocol, messages, parsing, routing and real-time devices for desktop and
embedded targets. Devices use Universal MIDI Packets internally; native
MIDI 1.0 and MIDI 2.0 messages can also be constructed independently of UMP. It is
built on Foundation (general utilities), DspCore (signal processing) and MCC
(music theory). It never throws
exceptions, and memory use is the implementer's choice (SPEC-RT-1).

> **Version 0.4.0** separates native Midi/Midi2 messages from Ump and restores
> the MIDI protocol guides and examples. See CHANGELOG.md for API migration.
> Stock Arduino C++11 source builds are supported. MIDI-CI is deferred.

## Features

- **Protocol** (`MIDILAR::Protocol`): channels, groups and their masks;
  note, controller and program numbers; velocity, controller, pressure and
  pitch-bend values at MIDI 2.0 resolution with MIDI 1.0 views; UMP
  `Packet`s; MIDI 1.0 byte-stream parsing and encoding with running status
  and System Exclusive; USB MIDI 1.0 event packets; MIDI Time Code; and note
  number / `MCC::Pitch` conversions. Native `Midi::Message` uses three inline
  bytes; `Midi::SysExMessage` uses caller storage, with explicit UMP bridging.
  The historical MIDI protocol topics are adapted to the current API.
- **Devices** (`MIDILAR::Devices`): channel filter and reassignment,
  transposition, velocity curves, scale filter and chord generation on MCC,
  a router, MIDI clock generator and receiver, a Euclidean step sequencer,
  and MTC generator and receiver. Devices chain through callbacks and never
  leave notes hanging.
- Hardware transports (UART, USB stacks, desktop MIDI APIs) are separate
  libraries built on these conversions. MIDI-CI is deferred.

Protocol is organized by representation:

| Namespace | Owns |
| --- | --- |
| `Protocol` | Shared channels/masks, note/controller/program numbers, status semantics, time code and scaling |
| `Protocol::Midi` | Native three-byte Message and caller-buffer SysExMessage |
| `Protocol::Midi2` | Native eight-byte channel-voice Message and high-resolution values |
| `Protocol::Ump` | Packet, groups/masks, utility/SysEx statuses, stream/USB adapters and explicit conversions |

Native message headers do not depend on UMP. Use Ump::Encode(group, message),
Ump::DecodeMidi1(packet) and Ump::DecodeMidi2(packet) for explicit conversion.
Packet and adapter headers now live under MIDILAR/Protocol/Ump; high-resolution
values under MIDILAR/Protocol/Midi2/Values.h. Old root aliases and MIDI 1.0
ToPacket/FromPacket members are removed; see the UMP Doxygen migration guide.

Clone with the pinned build infrastructure:

```bash
git clone --recurse-submodules https://github.com/ChecheRomo96/MIDILAR.git
```

For an existing checkout:

```bash
git submodule update --init --recursive
```

## Dependencies

MIDILAR links `Foundation::Foundation` (Foundation `2.0.5` or a newer `2.x`,
built on CPSTL `1.1.5`), `DspCore::DspCore` (DspCore `0.1.1` or a newer `0.x`)
and `MCC::MCC` (MCC `0.6.0` or a newer `0.x`). Arduino users install CPSTL,
Foundation, DspCore and MCC next to MIDILAR. Configuring resolves each one from
a parent project, an explicit prefix, a sibling export, normal `find_package`,
and finally the pinned GitHub Release package or the sources at that tag for
AVR and Arm presets.

## Build and test

The presets and scripts are the supported interface. Use the matching
PowerShell script on Windows.

```bash
./scripts/test.sh macos_arm64 --config Debug
./scripts/test.sh macos_arm64 --config Release
./scripts/build.sh macos_arm64 --config Release --examples-on
./scripts/test-package.sh macos_arm64
./scripts/export.sh atmega328p_avrgcc_avr5
./scripts/test-arduino.sh
./scripts/analyze.sh macos_arm64
./scripts/docs.sh
```

The generated documentation starts at
`build/documentation/docs/html/index.html`.

## Arduino

See examples/README.md for the desktop and Arduino
example catalog, including protocol messages, translations and routing.

Install CPSTL, Foundation, DspCore, MCC and MIDILAR as Arduino libraries, then include
MIDILAR, or only the modules the sketch uses. Every MIDILAR header also brings
in Foundation, DspCore and MCC, so the Arduino builder finds all four libraries:

```cpp
#include <MIDILAR.h>          // every module
#include <MIDILAR_Devices.h>  // or only the devices module
```

MIDILAR Arduino source builds support stock C++11 cores, including the Arduino
AVR core, without compiler-flag overrides. CMake and direct-source builds
continue to require C++17.

## License

Copyright (c) 2026 José Manuel Romo. All rights reserved.

MIDILAR is currently proprietary. No permission is granted for external use,
compilation, modification, redistribution, integration, or commercial use
without prior written authorization. See [LICENSE](LICENSE).

Shared embedded build support is pinned to RoModularBuild `v0.6.2`.
The `atmega328p_avrgcc_avr5`, `atmega2560_avrgcc_avr6` and
`stm32g0b1cbt6_armgcc_cortex_m0plus_soft` presets retain repository-specific
feature and package policy. CI compile/package checks do not establish physical
board execution.
