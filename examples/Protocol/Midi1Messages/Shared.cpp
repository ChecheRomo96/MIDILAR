#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi1Messages {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi1Messages ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "MIDI 1.0: siete familias de mensajes, grupo 1 y canal 1.\nCada mensaje muestra su significado, campos y UMP.\nSolo inspeccionamos datos; no se envian a un sintetizador.\n\n");
    const Group group = Group::FromNumber(1);
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    const Packet messages[] = {
        Packet::Midi1NoteOn(group, channel, note, Velocity::FromMidi1(100)),
        Packet::Midi1PolyPressure(group, channel, note, PressureValue::FromMidi1(50)),
        Packet::Midi1ControlChange(group, channel, ControllerNumber::FromValue(1), ControllerValue::FromMidi1(64)),
        Packet::Midi1ProgramChange(group, channel, ProgramNumber::FromValue(10)),
        Packet::Midi1ChannelPressure(group, channel, PressureValue::FromMidi1(70)),
        Packet::Midi1PitchBend(group, channel, PitchBend::Center()),
        Packet::Midi1NoteOff(group, channel, note, Velocity::Min())
    };
    // Each message is a 32-bit UMP; encoding removes the UMP header.
    for (const Packet& packet : messages) PrintPacket(print, packet);
}
}
