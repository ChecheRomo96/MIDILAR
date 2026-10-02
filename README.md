# MIDILAR

MIDILAR is the MIDI layer of the RoModular ecosystem: MIDI 1.0 and MIDI 2.0
protocol, messages, parsing, routing and real-time devices for desktop and
embedded targets. MIDI 2.0 Universal MIDI Packets are its internal model;
MIDI 1.0 byte streams are translated at the edges. It is
built on Foundation (general utilities) and MCC (music theory), and it never
allocates memory or throws exceptions in real-time paths.

> **Status: 0.2.0, rebuilt from scratch.** The first release of the rebuilt
> library is prepared on the `rebuild` branch and merges into `main` after
> hardware validation. Until then, add `-b rebuild` to the clone command.

## Features

- **Protocol** (`MIDILAR::Protocol`): channels, groups and their masks;
  note, controller and program numbers; velocity, controller, pressure and
  pitch-bend values at MIDI 2.0 resolution with MIDI 1.0 views; UMP
  `Packet`s; MIDI 1.0 byte-stream parsing and encoding with running status
  and System Exclusive; USB MIDI 1.0 event packets; MIDI Time Code; and note
  number / `MCC::Pitch` conversions.
- **Devices** (`MIDILAR::Devices`): channel filter and reassignment,
  transposition, velocity curves, scale filter and chord generation on MCC,
  a router, MIDI clock generator and receiver, a Euclidean step sequencer,
  and MTC generator and receiver. Devices chain through callbacks, never
  allocate and never leave notes hanging.
- Hardware transports (UART, USB stacks, desktop MIDI APIs) are separate
  libraries built on these conversions. MIDI-CI is deferred.

Clone with the pinned build infrastructure:

```bash
git clone --recurse-submodules https://github.com/ChecheRomo96/MIDILAR.git
```

For an existing checkout:

```bash
git submodule update --init --recursive
```

## Dependencies

MIDILAR links `Foundation::Foundation` (Foundation `1.4.0` or a newer `1.x`)
and `MCC::MCC` (MCC `0.5.2` or a newer `0.x`). Configuring resolves each one
from a parent project, an explicit prefix (`MIDILAR_FOUNDATION_PREFIX`,
`MIDILAR_MCC_PREFIX`), a sibling export in `../Foundation/dist/<preset>` or
`../MCC/dist/<preset>`, normal `find_package`, and finally the pinned GitHub
Release package, or the sources at that tag for AVR and Arm presets.

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

Install Foundation, MCC and MIDILAR as Arduino libraries, then include all
three from the sketch itself, because the Arduino builder only discovers
libraries that the sketch includes:

```cpp
#include <Foundation.h>
#include <MCC.h>
#include <MIDILAR.h>
```

MIDILAR requires C++17. On the stock Arduino AVR core add `-std=gnu++17`, for
example with
`arduino-cli compile --build-property "compiler.cpp.extra_flags=-std=gnu++17"`.

## License

Copyright (c) 2026 José Manuel Romo. All rights reserved.

MIDILAR is currently proprietary. No permission is granted for external use,
compilation, modification, redistribution, integration, or commercial use
without prior written authorization. See [LICENSE](LICENSE).
