// MIDI Time Code through the whole MIDI 1.0 path on the board, without a MIDI
// interface: generator -> bytes -> parser -> receiver. Prints the received
// time once per second on the serial monitor at 115200 baud. Each line should
// be one second (25 frames) later than the previous one, within a frame,
// because a full quarter-frame message takes two frames to arrive.
//
// The Arduino builder only discovers libraries included from the sketch.
#include <Foundation.h>
#include <MCC.h>
#include <MIDILAR.h>

#include "Shared.h"

// Oscillator correction in parts per million; see ClockOut.
#ifndef MIDILAR_EXAMPLE_CLOCK_PPM
    #define MIDILAR_EXAMPLE_CLOCK_PPM 0
#endif

namespace {

uint32_t Micros() {
    return micros();
}

MIDILARExamples::Devices::MtcLoopback::MtcLoopback loopback(Micros, MIDILAR_EXAMPLE_CLOCK_PPM);
uint32_t next = 1000;

} // namespace

void setup() {
    Serial.begin(115200);
    loopback.Start(MIDILAR::Protocol::TimeCode::From(1, 0, 0, 0, MIDILAR::Protocol::TimeCodeRate::Fps25));
}

void loop() {
    loopback.Update();
    if (static_cast<int32_t>(millis() - next) >= 0) {
        next += 1000;
        const MIDILAR::Protocol::TimeCode t = loopback.Received();
        char line[40];
        snprintf(line, sizeof line, "%02u:%02u:%02u:%02u  (%lu bytes)", t.Hours(), t.Minutes(), t.Seconds(),
                 t.Frames(), static_cast<unsigned long>(loopback.Bytes()));
        Serial.println(line);
    }
}
