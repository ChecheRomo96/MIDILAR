#include "Shared.h"

namespace MIDILARExamples::Devices::ClockOut {

ClockOut::ClockOut(uint32_t (*micros)()) noexcept
    : _clock(micros, Foundation::Time::Frequency(1000000, 1)), _generator(_clock) {
    _sequencer.SetPattern(MIDILAR::Devices::EuclideanPattern(3, 8), 8);
    _sequencer.SetNote(MIDILAR::Protocol::NoteNumber::FromValue(36));
    _generator.Output().Bind<MIDILAR::Devices::StepSequencer, &MIDILAR::Devices::StepSequencer::Process>(
        &_sequencer);
    _sequencer.Output().Bind<ClockOut, &ClockOut::Send>(this);
}

void ClockOut::Start(uint32_t centiBpm) {
    _generator.SetTempo(centiBpm);
    _generator.Start();
}

void ClockOut::Update(void (*write)(uint8_t)) {
    _write = write;
    _generator.Update();
}

uint32_t ClockOut::Position() const noexcept {
    return _generator.Position();
}

void ClockOut::Send(const MIDILAR::Protocol::Packet& packet) {
    uint8_t bytes[MIDILAR::Protocol::Midi1Encoder::MaxBytes];
    const uint8_t count = _encoder.Encode(packet, bytes);
    for (uint8_t i = 0; i < count && _write != nullptr; ++i) {
        _write(bytes[i]);
    }
}

} // namespace MIDILARExamples::Devices::ClockOut
