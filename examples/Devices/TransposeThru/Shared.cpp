#include "Shared.h"

#include <MCC/Scale/Scales.h>

namespace MIDILARExamples::Devices::TransposeThru {

using MIDILAR::Protocol::Packet;

Thru::Thru() noexcept {
    _scale.SetScale(MCC::Scales::Make(MCC::NoteName(MCC::Letter::C, MCC::Accidental::Natural()),
                                      MCC::Scales::Id::Major));
    _transpose.Output().Bind<MIDILAR::Devices::ScaleFilter, &MIDILAR::Devices::ScaleFilter::Process>(&_scale);
    _scale.Output().Bind<Thru, &Thru::Collect>(this);
}

void Thru::SetSemitones(int32_t semitones) noexcept {
    _transpose.SetSemitones(semitones);
}

uint8_t Thru::Process(uint8_t byte, uint8_t (&out)[Capacity]) {
    _out = out;
    _count = 0;
    Packet packets[MIDILAR::Protocol::Midi1Parser::MaxPackets];
    const uint8_t count = _parser.Parse(byte, packets);
    for (uint8_t i = 0; i < count; ++i) {
        _transpose.Process(packets[i]);
    }
    return _count;
}

void Thru::Collect(const Packet& packet) {
    uint8_t bytes[MIDILAR::Protocol::Midi1Encoder::MaxBytes];
    const uint8_t count = _encoder.Encode(packet, bytes);
    for (uint8_t i = 0; i < count && _count < Capacity; ++i) {
        _out[_count++] = bytes[i];
    }
}

} // namespace MIDILARExamples::Devices::TransposeThru
