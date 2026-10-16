#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi1Messages {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi1Messages ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "MIDI 1.0: siete familias de mensajes, grupo 1 y canal 1.\nCada mensaje muestra su significado, campos y UMP.\nSolo inspeccionamos datos; no se envian a un sintetizador.\n\n");
    const Ump::Group group = Ump::Group::FromNumber(1);
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    const Ump::Packet messages[] = {
        Ump::Packet::Midi1NoteOn(group, channel, note, Midi2::Velocity::FromMidi1(100)),
        Ump::Packet::Midi1PolyPressure(group, channel, note, Midi2::PressureValue::FromMidi1(50)),
        Ump::Packet::Midi1ControlChange(group, channel, ControllerNumber::FromValue(1), Midi2::ControllerValue::FromMidi1(64)),
        Ump::Packet::Midi1ProgramChange(group, channel, ProgramNumber::FromValue(10)),
        Ump::Packet::Midi1ChannelPressure(group, channel, Midi2::PressureValue::FromMidi1(70)),
        Ump::Packet::Midi1PitchBend(group, channel, Midi2::PitchBend::Center()),
        Ump::Packet::Midi1NoteOff(group, channel, note, Midi2::Velocity::Min())
    };
    // Each message is a 32-bit UMP; encoding removes the UMP header.
    for (const Ump::Packet& packet : messages) PrintPacket(print, packet);
}
}
