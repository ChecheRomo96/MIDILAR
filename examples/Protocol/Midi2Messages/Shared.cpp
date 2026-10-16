#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi2Messages {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi2Messages ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "MIDI 2.0: mensajes con mayor resolucion.\nNote On usa velocity 50000. Program Change incluye banco 130.\nAl convertir a MIDI 1.0 se reduce la resolucion.\n\n");
    const Group group = Group::FromNumber(1);
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    const Packet messages[] = {
        Packet::Midi2NoteOn(group, channel, note, Velocity::FromMidi2(50000)),
        Packet::Midi2PolyPressure(group, channel, note, PressureValue::FromMidi2(0x90000000u)),
        Packet::Midi2ControlChange(group, channel, ControllerNumber::FromValue(74), ControllerValue::FromMidi2(0xA0000000u)),
        Packet::Midi2ProgramChange(group, channel, ProgramNumber::FromValue(10), 130),
        Packet::Midi2ChannelPressure(group, channel, PressureValue::Center()),
        Packet::Midi2PitchBend(group, channel, PitchBend::Center()),
        Packet::Midi2NoteOff(group, channel, note, Velocity::Min())
    };
    for (const Packet& packet : messages) PrintPacket(print, packet);
    // MIDI 1.0 transports need reduced resolution; UMP transports keep both words.
    Midi1Encoder encoder;
    uint8_t bytes[Midi1Encoder::MaxBytes];
    MIDILAR_EXAMPLE_PRINT(print, "\nConversion del primer Note On a MIDI 1.0:\n");
    const uint8_t count = encoder.Encode(messages[0], bytes);
    PrintNumber(print, "Cantidad de bytes (esperado 3)", count);
    for (uint8_t i = 0; i < count; ++i) PrintByte(print, "Byte MIDI 1.0", bytes[i]);
}
}
