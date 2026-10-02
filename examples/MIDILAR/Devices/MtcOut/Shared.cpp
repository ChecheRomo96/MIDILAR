#include "Shared.h"

namespace MIDILARExamples::Devices::MtcOut {

MtcOut::MtcOut(uint32_t (*micros)()) noexcept
    : _clock(micros, Foundation::Time::Frequency(1000000, 1)), _generator(_clock) {
    _generator.Output().Bind<MtcOut, &MtcOut::Send>(this);
}

void MtcOut::Start(MIDILAR::Protocol::TimeCode time, void (*write)(uint8_t)) {
    _write = write;
    _generator.Locate(time);
    _generator.Start();
}

void MtcOut::Update(void (*write)(uint8_t)) {
    _write = write;
    _generator.Update();
}

MIDILAR::Protocol::TimeCode MtcOut::Time() const noexcept {
    return _generator.Time();
}

void MtcOut::Send(const MIDILAR::Protocol::Packet& packet) {
    uint8_t bytes[MIDILAR::Protocol::Midi1Encoder::MaxBytes];
    const uint8_t count = _encoder.Encode(packet, bytes);
    for (uint8_t i = 0; i < count && _write != nullptr; ++i) {
        _write(bytes[i]);
    }
}

} // namespace MIDILARExamples::Devices::MtcOut
