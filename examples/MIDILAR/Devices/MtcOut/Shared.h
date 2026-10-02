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
    /** `micros` returns a free-running microsecond counter. */
    explicit MtcOut(uint32_t (*micros)()) noexcept;

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
