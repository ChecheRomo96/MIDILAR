#include "Shared.h"

namespace MIDILARExamples::Protocol::SystemMessages {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== SystemMessages ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Sistema: transporte y sincronizacion.\nUtility: mensajes propios de UMP, sin bytes MIDI 1.0.\nSong Position 16 equivale a 96 clocks (4 negras).\n\n");
    const Ump::Group group = Ump::Group::FromNumber(1);
    const Ump::Packet messages[] = {
        Ump::Packet::System(group, SystemStatus::Start),
        Ump::Packet::System(group, SystemStatus::TimingClock),
        Ump::Packet::System(group, SystemStatus::Stop),
        Ump::Packet::SongPosition(group, 16),
        Ump::Packet::System(group, SystemStatus::Continue),
        Ump::Packet::SongSelect(group, 2),
        Ump::Packet::System(group, SystemStatus::TuneRequest),
        Ump::Packet::TimeCode(group, 0x24),
        Ump::Packet::Noop(), Ump::Packet::JrClock(100), Ump::Packet::JrTimestamp(101),
        Ump::Packet::TicksPerQuarterNote(96), Ump::Packet::DeltaClockstamp(24)
    };
    // Utility packets have no MIDI 1.0 byte representation.
    Ump::Midi1Encoder encoder;
    for (const Ump::Packet& packet : messages) {
        PrintPacket(print, packet);
        uint8_t bytes[Ump::Midi1Encoder::MaxBytes];
        const uint8_t count = encoder.Encode(packet, bytes);
        PrintNumber(print, "Bytes para transporte MIDI 1.0", count);
        if (count == 0) MIDILAR_EXAMPLE_PRINT(print, "Sin equivalente MIDI 1.0: conservar como UMP.\n");
        for (uint8_t i = 0; i < count; ++i) PrintByte(print, "Byte MIDI 1.0", bytes[i]);
    }
}
}
