#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi1Encoding {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi1Encoding ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Encoder con running status: omitir status repetidos.\nEsperado por mensaje: 3, 1, 2, 3, 2 bytes.\nClock conserva el status; pasar de Note On a Note Off lo cambia.\n\n");
    const Ump::Group group = Ump::Group::FromNumber(1);
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    Ump::Midi1Encoder encoder;
    encoder.SetRunningStatus(true);
    const Ump::Packet messages[] = {
        Ump::Packet::Midi1NoteOn(group, channel, note, Midi2::Velocity::FromMidi1(100)),
        Ump::Packet::System(group, SystemStatus::TimingClock),
        Ump::Packet::Midi1NoteOn(group, channel, NoteNumber::FromValue(62), Midi2::Velocity::FromMidi1(110)),
        Ump::Packet::Midi1NoteOff(group, channel, note, Midi2::Velocity::Min()),
        Ump::Packet::Midi1NoteOff(group, channel, NoteNumber::FromValue(62), Midi2::Velocity::Min())
    };
    // The clock preserves running status; changing Note On to Note Off replaces it.
    for (const Ump::Packet& packet : messages) {
        PrintPacket(print, packet);
        uint8_t bytes[Ump::Midi1Encoder::MaxBytes];
        const uint8_t count = encoder.Encode(packet, bytes);
        PrintNumber(print, "Cantidad de bytes emitidos", count);
        for (uint8_t i = 0; i < count; ++i) PrintByte(print, "Byte MIDI 1.0", bytes[i]);
    }
}
}
