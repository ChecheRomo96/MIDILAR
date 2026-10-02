#ifndef MIDILAR_EXAMPLES_DEVICES_CLOCK_OUT_SHARED_H
#define MIDILAR_EXAMPLES_DEVICES_CLOCK_OUT_SHARED_H

#include <stdint.h>

#include <Foundation/Time/Clock.h>
#include <MIDILAR/Devices/MidiClock.h>
#include <MIDILAR/Devices/Sequencer.h>
#include <MIDILAR/Protocol/Midi1Encoder.h>

namespace MIDILARExamples::Devices::ClockOut {

/**
 * Sends MIDI 1.0 clock bytes at a tempo, timed by a microsecond clock, and a
 * Euclidean E(3, 8) rhythm on C2 in sixteenth notes.
 */
class ClockOut {
public:
    /** `micros` returns a free-running microsecond counter. */
    explicit ClockOut(uint32_t (*micros)()) noexcept;

    /** Starts playing at `centiBpm` hundredths of a BPM. */
    void Start(uint32_t centiBpm);

    /** Calls `write(byte)` for every byte that is due. */
    void Update(void (*write)(uint8_t));

    /** MIDI clocks played since Start. */
    uint32_t Position() const noexcept;

private:
    void Send(const MIDILAR::Protocol::Packet& packet);

    Foundation::Time::Clock _clock;
    MIDILAR::Devices::ClockGenerator _generator;
    MIDILAR::Devices::StepSequencer _sequencer;
    MIDILAR::Protocol::Midi1Encoder _encoder;
    void (*_write)(uint8_t) = nullptr;
};

} // namespace MIDILARExamples::Devices::ClockOut

#endif // MIDILAR_EXAMPLES_DEVICES_CLOCK_OUT_SHARED_H
