# MIDILAR examples

Each example contains portable Shared.h / Shared.cpp, a Desktop adapter and
an Arduino sketch. The fourteen demonstrations below print text and require no
MIDI hardware. They use the current Packet API.

## Build and run

From the repository root on Windows:

```powershell
./scripts/build.ps1 windows_msvc_x64 -ExamplesOn
& ./build/windows_msvc_x64/bin/Debug/MIDILAR_Protocol_Midi1Messages_d.exe
```

For macOS use `./scripts/build.sh macos_arm64 --examples-on`.
Use `cmake --list-presets` to find the native preset for other platforms.
Examples are included in the existing Examples install/export component.

For Arduino, install CPSTL, Foundation, DspCore, MCC and MIDILAR, open the
example's .ino, upload and open Serial Monitor at 115200 baud. Each sketch
runs once in setup; restart to repeat. Each folder can be copied independently.
Use scripts/test-arduino.ps1 (or .sh) to compile all sketches with arduino-cli.

## Catalog

| Example | Demonstrates |
| --- | --- |
| [Protocol/CreatingMessages](Protocol/CreatingMessages/CreatingMessages.ino) | Native three-byte Message, validation, pitch bend and explicit UMP bridge |
| [Protocol/ControlChanges](Protocol/ControlChanges/ControlChanges.ino) | Native CC helpers, channel modes and an explicit four-message NRPN sequence |
| [Protocol/SysExBuffer](Protocol/SysExBuffer/SysExBuffer.ino) | Caller-owned SysEx bytes, capacity failure and ten-byte MTC Full Frame |
| [Protocol/Values](Protocol/Values/Values.ino) | Validated channels and notes, velocity scaling and pitch-bend center |
| [Protocol/Midi1Messages](Protocol/Midi1Messages/Midi1Messages.ino) | All seven MIDI 1.0 channel voice message families as UMP |
| [Protocol/Midi2Messages](Protocol/Midi2Messages/Midi2Messages.ino) | All seven supported MIDI 2.0 channel voice families and MIDI 1.0 translation |
| [Protocol/Midi1Parsing](Protocol/Midi1Parsing/Midi1Parsing.ino) | Running status, real-time interleaving and velocity-zero Note Off |
| [Protocol/Midi1Encoding](Protocol/Midi1Encoding/Midi1Encoding.ino) | Running-status encoding with interleaved clock and status changes |
| [Protocol/SysEx](Protocol/SysEx/SysEx.ino) | Caller-owned payload buffer, multi-packet assembly and explicit truncation |
| [Protocol/UsbMidi1](Protocol/UsbMidi1/UsbMidi1.ino) | USB MIDI 1.0 event decoding and encoding with cable-to-group mapping |
| [Protocol/NotePitch](Protocol/NotePitch/NotePitch.ino) | MCC pitch conversion, middle C and out-of-range rejection |
| [Protocol/SystemMessages](Protocol/SystemMessages/SystemMessages.ino) | System common, real-time and UMP utility messages |
| [Devices/Routing](Devices/Routing/Routing.ino) | Fan-out router, channel filtering and channel reassignment |
| [Devices/HeldNotes](Devices/HeldNotes/HeldNotes.ino) | Safe Note Off after changing a transpose mapping |

Console explanations are in Spanish (ASCII for serial-monitor compatibility).
On AVR, explanatory text literals are streamed from program memory in small
chunks to leave RAM available for processing. All 20 sketches, including the
existing timing examples, compile for the stock Arduino Uno core.
Each demo introduces the operation and expected result. Packets show named
message types, group/channel numbers, notes, velocity, controller, program,
pressure or pitch-bend fields as applicable, followed by the raw UMP words.
Parser and encoder demos trace incoming/outgoing bytes in hexadecimal and
decimal. SysEx explains truncation; routing and held-note demos identify
branches and mapping changes.

UMP output prints each 32-bit word in hexadecimal, in protocol word order.
Other numeric output uses decimal unless explicitly labelled hexadecimal.

## Expected results

- Values: channel 1 has wire address 0, velocity round trip is 100, pitch bend
  center is 8192; channel 17 and note 128 are rejected.
- Midi1Messages: seven one-word messages, starting with 20903C64 and ending
  with 20803C00 (note 60 on/off).
- Midi2Messages: seven two-word messages; the first Note On translates to
  three MIDI 1.0 bytes at reduced resolution.
- Midi1Parsing: four packets, in order: note 60 on, clock, note 62 on,
  note 60 off. Velocity-zero Note On is normalized to Note Off.
- Midi1Encoding: message byte counts 3, 1, 2, 3, 2; interleaved clock
  preserves running status.
- SysEx: eight stored payload bytes, Truncado: 1; clock is excluded from
  the payload. Read Data/Size/IsTruncated before the next Push, and use a
  larger caller-owned buffer to retain the entire message.
- UsbMidi1: one event with bytes 25, 144, 60, 100. Cable wire index 1 maps
  to group number 2.
- NotePitch: MIDI 60 is C4 and round trips to 60; index 128 is rejected.
- SystemMessages: system common/real-time messages have MIDI 1.0 bytes;
  UMP utility messages produce zero MIDI 1.0 bytes.
- Routing: channel 1 notes reach both branches; channel 2 notes reach only
  the reassigned branch, which outputs channel 10.
- HeldNotes: notes 72, 72, 48, 48; the first Note Off keeps its Note On's
  transposition despite a subsequent parameter change.

## Existing timing and hardware examples

- Core/Version prints the complete dependency chain.
- Devices/ClockOut combines MIDI clock and Euclidean sequencing.
- Devices/CalibrateClock measures oscillator correction.
- Devices/MtcOut and Devices/MtcLoopback demonstrate time code.
- Devices/TransposeThru combines transposition and scale filtering.

The text demonstrations above must not feed a MIDI input. Existing MIDI UART
examples send binary bytes through a 115200-baud bridge; DIN MIDI shields
require the documented 31250-baud override. Compile/link checks do not
establish physical-board execution.
