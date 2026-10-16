#include "Shared.h"

namespace MIDILARExamples::Devices::MtcLoopback {

using MIDILAR::Protocol::Ump::Packet;

MtcLoopback::MtcLoopback(uint32_t (*micros)(), int32_t calibrationPpm) noexcept
    : _clock(micros, Foundation::Time::Frequency(static_cast<uint32_t>(1000000 + calibrationPpm), 1)),
      _generator(_clock) {
    _generator.Output().Bind<MtcLoopback, &MtcLoopback::Send>(this);
}

void MtcLoopback::Start(MIDILAR::Protocol::TimeCode time) {
    _generator.Locate(time);
    _generator.Start();
}

void MtcLoopback::Update() {
    _generator.Update();
}

MIDILAR::Protocol::TimeCode MtcLoopback::Received() const noexcept {
    return _receiver.Time();
}

uint32_t MtcLoopback::Bytes() const noexcept {
    return _bytes;
}

void MtcLoopback::Send(const MIDILAR::Protocol::Ump::Packet& packet) {
    uint8_t bytes[MIDILAR::Protocol::Ump::Midi1Encoder::MaxBytes];
    const uint8_t count = _encoder.Encode(packet, bytes);
    for (uint8_t i = 0; i < count; ++i) {
        ++_bytes;
        MIDILAR::Protocol::Ump::Packet parsed[MIDILAR::Protocol::Ump::Midi1Parser::MaxPackets];
        const uint8_t packets = _parser.Parse(bytes[i], parsed);
        for (uint8_t j = 0; j < packets; ++j) {
            _receiver.Process(parsed[j]);
        }
    }
}

} // namespace MIDILARExamples::Devices::MtcLoopback
