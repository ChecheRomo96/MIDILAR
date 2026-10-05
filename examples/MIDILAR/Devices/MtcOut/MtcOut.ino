// Sends MIDI Time Code at 25 fps from 01:00:00:00 on the Uno's serial port,
// for a MIDI shield: a full frame, then four quarter frames per frame.

#include <MIDILAR_Devices.h>

#include "Shared.h"

// 31250 baud drives a MIDI shield. To test over the USB cable with a
// serial-to-MIDI bridge (Hairless MIDI) instead, compile with
// -DMIDILAR_EXAMPLE_MIDI_BAUD=115200.
#ifndef MIDILAR_EXAMPLE_MIDI_BAUD
    #define MIDILAR_EXAMPLE_MIDI_BAUD 31250
#endif

// Oscillator correction in parts per million. If the receiver measures this
// board's tempo or time code running slow, compile with a negative value,
// e.g. -DMIDILAR_EXAMPLE_CLOCK_PPM=-500 when 120 BPM reads as 119.94.
#ifndef MIDILAR_EXAMPLE_CLOCK_PPM
    #define MIDILAR_EXAMPLE_CLOCK_PPM 0
#endif

namespace {

uint32_t Micros() {
    return micros();
}

void Write(uint8_t byte) {
    Serial.write(byte);
}

MIDILARExamples::Devices::MtcOut::MtcOut out(Micros, MIDILAR_EXAMPLE_CLOCK_PPM);

} // namespace

void setup() {
    Serial.begin(MIDILAR_EXAMPLE_MIDI_BAUD);
    out.Start(MIDILAR::Protocol::TimeCode::From(1, 0, 0, 0, MIDILAR::Protocol::TimeCodeRate::Fps25), Write);
}

void loop() {
    out.Update(Write);
}
