#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi2Messages {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi2Messages ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Midi2::Message: mensajes nativos de 8 bytes, sin grupo ni cabecera UMP.\nVelocity usa 16 bits; controladores, presion y bend usan 32 bits.\n");
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    const Midi2::Message messages[] = {
        Midi2::Message::NoteOn(channel, note, Midi2::Velocity::FromMidi2(50000)),
        Midi2::Message::PolyPressure(channel, note, Midi2::PressureValue::FromMidi2(0x90000000u)),
        Midi2::Message::ControlChange(channel, ControllerNumber::FromValue(74), Midi2::ControllerValue::FromMidi2(0xA0000000u)),
        Midi2::Message::ProgramChange(channel, ProgramNumber::FromValue(10), 130),
        Midi2::Message::ChannelPressure(channel, Midi2::PressureValue::Center()),
        Midi2::Message::PitchBend(channel, Midi2::PitchBend::Center()),
        Midi2::Message::NoteOff(channel, note, Midi2::Velocity::Min())
    };
    PrintNumber(print, "sizeof(Midi2::Message), esperado 8", static_cast<uint32_t>(sizeof(Midi2::Message)));
    PrintNumber(print, "Velocity nativa, esperado 50000", messages[0].Velocity().Midi2());
    MIDILAR_EXAMPLE_PRINT(print, "\nUmp::Encode agrega explicitamente grupo 1 y tipo MIDI 2.0.\n");
    const Ump::Group group = Ump::Group::FromNumber(1);
    for (const auto& message : messages) {
        const auto packet = Ump::Encode(group, message);
        PrintPacket(print, packet);
        PrintNumber(print, "Round trip nativo exacto (1=si)", Ump::DecodeMidi2(packet) == message);
    }
    MIDILAR_EXAMPLE_PRINT(print, "\nTraduccion explicita a bytes MIDI 1.0: reduce resolucion.\n");
    Ump::Midi1Encoder encoder;
    uint8_t bytes[Ump::Midi1Encoder::MaxBytes];
    const uint8_t count = encoder.Encode(Ump::Encode(group, messages[0]), bytes);
    PrintNumber(print, "Cantidad de bytes (esperado 3)", count);
    for (uint8_t i = 0; i < count; ++i) PrintByte(print, "Byte MIDI 1.0", bytes[i]);
}
}
