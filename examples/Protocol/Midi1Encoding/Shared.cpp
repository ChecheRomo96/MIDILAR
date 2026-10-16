#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi1Encoding {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi1Encoding ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Encoder con running status: omitir status repetidos.\nEsperado por mensaje: 3, 1, 2, 3, 2 bytes.\nClock conserva el status; pasar de Note On a Note Off lo cambia.\n\n");
    const Group group = Group::FromNumber(1);
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    Midi1Encoder encoder;
    encoder.SetRunningStatus(true);
    const Packet messages[] = {
        Packet::Midi1NoteOn(group, channel, note, Velocity::FromMidi1(100)),
        Packet::System(group, SystemStatus::TimingClock),
        Packet::Midi1NoteOn(group, channel, NoteNumber::FromValue(62), Velocity::FromMidi1(110)),
        Packet::Midi1NoteOff(group, channel, note, Velocity::Min()),
        Packet::Midi1NoteOff(group, channel, NoteNumber::FromValue(62), Velocity::Min())
    };
    // The clock preserves running status; changing Note On to Note Off replaces it.
    for (const Packet& packet : messages) {
        PrintPacket(print, packet);
        uint8_t bytes[Midi1Encoder::MaxBytes];
        const uint8_t count = encoder.Encode(packet, bytes);
        PrintNumber(print, "Cantidad de bytes emitidos", count);
        for (uint8_t i = 0; i < count; ++i) PrintByte(print, "Byte MIDI 1.0", bytes[i]);
    }
}
}
