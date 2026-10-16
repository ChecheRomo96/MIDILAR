#ifndef MIDILAR_EXAMPLES_DEVICES_MTC_OUT_SHARED_H
#define MIDILAR_EXAMPLES_DEVICES_MTC_OUT_SHARED_H

#include <stdint.h>

#include <Foundation/Time/Clock.h>
#include <MIDILAR/Devices/Mtc.h>
#include <MIDILAR/Protocol/Midi1Encoder.h>

namespace MIDILARExamples::Devices::MtcOut {

/**
 * Sends MIDI Time Code as MIDI 1.0 bytes, timed by a microsecond clock: a
 * full frame, then four quarter frames per frame.
 */
class MtcOut {
public:
    /**
     * `micros` returns a free-running microsecond counter. `calibrationPpm`
     * corrects the board's oscillator: a board whose clock runs 500 ppm slow
     * (120 BPM measured as 119.94) uses -500.
     */
    explicit MtcOut(uint32_t (*micros)(), int32_t calibrationPpm = 0) noexcept;

    /** Moves to `time`, sends it as a full frame and starts running. */
    void Start(MIDILAR::Protocol::TimeCode time, void (*write)(uint8_t));

    /** Calls `write(byte)` for every byte that is due. */
    void Update(void (*write)(uint8_t));

    /** The frame being played. */
    MIDILAR::Protocol::TimeCode Time() const noexcept;

private:
    void Send(const MIDILAR::Protocol::Packet& packet);

    Foundation::Time::Clock _clock;
    MIDILAR::Devices::MtcGenerator _generator;
    MIDILAR::Protocol::Midi1Encoder _encoder;
    void (*_write)(uint8_t) = nullptr;
};

} // namespace MIDILARExamples::Devices::MtcOut

#endif // MIDILAR_EXAMPLES_DEVICES_MTC_OUT_SHARED_H
