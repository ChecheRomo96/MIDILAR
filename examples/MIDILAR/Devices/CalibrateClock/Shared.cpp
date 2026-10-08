#include "Shared.h"

namespace MIDILARExamples::Devices::CalibrateClock {

CalibrateClock::CalibrateClock(uint32_t (*micros)()) noexcept
    : _clock(micros, Foundation::Time::Frequency(1000000, 1)), _calibrator(_clock) {}

void CalibrateClock::Start(uint32_t centiBpm, uint32_t clocks) {
    _calibrator.Start(centiBpm, clocks);
}

void CalibrateClock::Receive(uint8_t byte) {
    MIDILAR::Protocol::Packet packets[MIDILAR::Protocol::Midi1Parser::MaxPackets];
    const uint8_t count = _parser.Parse(byte, packets);
    for (uint8_t i = 0; i < count; ++i) {
        _calibrator.Process(packets[i]);
    }
}

bool CalibrateClock::IsDone() const noexcept {
    return _calibrator.IsDone();
}

int32_t CalibrateClock::Ppm() const noexcept {
    return _calibrator.Ppm();
}

} // namespace MIDILARExamples::Devices::CalibrateClock
