#ifndef MIDILAR_EXAMPLES_DEVICES_MTC_LOOPBACK_SHARED_H
#define MIDILAR_EXAMPLES_DEVICES_MTC_LOOPBACK_SHARED_H

#include <stdint.h>

#include <Foundation/Time/Clock.h>
#include <MIDILAR/Devices/Mtc.h>
#include <MIDILAR/Protocol/Midi1Encoder.h>
#include <MIDILAR/Protocol/Midi1Parser.h>

namespace MIDILARExamples::Devices::MtcLoopback {

/**
 * MIDI Time Code through the whole MIDI 1.0 path on one device, without a
 * MIDI interface: MtcGenerator -> Midi1Encoder -> bytes -> Midi1Parser ->
 * MtcReceiver.
 */
class MtcLoopback {
public:
    /** `micros` returns a free-running microsecond counter; see ClockOut for `calibrationPpm`. */
    explicit MtcLoopback(uint32_t (*micros)(), int32_t calibrationPpm = 0) noexcept;

    /** Moves to `time` and starts sending quarter frames. */
    void Start(MIDILAR::Protocol::TimeCode time);

    /** Sends the quarter frames that are due through the loop. */
    void Update();

    /** The time the receiver has decoded so far. */
    MIDILAR::Protocol::TimeCode Received() const noexcept;

    /** MIDI 1.0 bytes that went through the loop. */
    uint32_t Bytes() const noexcept;

private:
    void Send(const MIDILAR::Protocol::Packet& packet);

    Foundation::Time::Clock _clock;
    MIDILAR::Devices::MtcGenerator _generator;
    MIDILAR::Protocol::Midi1Encoder _encoder;
    MIDILAR::Protocol::Midi1Parser _parser;
    MIDILAR::Devices::MtcReceiver _receiver;
    uint32_t _bytes = 0;
};

} // namespace MIDILARExamples::Devices::MtcLoopback

#endif // MIDILAR_EXAMPLES_DEVICES_MTC_LOOPBACK_SHARED_H
