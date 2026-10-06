#ifndef MIDILAR_EXAMPLES_DEVICES_CALIBRATE_CLOCK_SHARED_H
#define MIDILAR_EXAMPLES_DEVICES_CALIBRATE_CLOCK_SHARED_H

#include <stdint.h>

#include <Foundation/Time/Clock.h>
#include <MIDILAR/Devices/MidiClock.h>
#include <MIDILAR/Protocol/Midi1Parser.h>

namespace MIDILARExamples::Devices::CalibrateClock {

/**
 * Measures this device's oscillator error against a reference MIDI clock of
 * known tempo, received as MIDI 1.0 bytes.
 */
class CalibrateClock {
public:
    /** `micros` returns a free-running microsecond counter. */
    explicit CalibrateClock(uint32_t (*micros)()) noexcept;

    /** Starts measuring a reference at `centiBpm` over `clocks` timing clocks. */
    void Start(uint32_t centiBpm, uint32_t clocks);

    /** Consumes one received MIDI byte. */
    void Receive(uint8_t byte);

    /** True once the measurement is complete. */
    bool IsDone() const noexcept;

    /** The correction in parts per million (-500 for a clock 500 ppm slow). */
    int32_t Ppm() const noexcept;

private:
    Foundation::Time::Clock _clock;
    MIDILAR::Protocol::Midi1Parser _parser;
    MIDILAR::Devices::ClockCalibrator _calibrator;
};

} // namespace MIDILARExamples::Devices::CalibrateClock

#endif // MIDILAR_EXAMPLES_DEVICES_CALIBRATE_CLOCK_SHARED_H
