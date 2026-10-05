// Sends MIDI clock at 120 BPM (Start, then 24 timing clocks per beat) and a
// Euclidean E(3, 8) rhythm on C2 on the Uno's serial port, for a MIDI shield.

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

MIDILARExamples::Devices::ClockOut::ClockOut out(Micros, MIDILAR_EXAMPLE_CLOCK_PPM);

} // namespace

void setup() {
    Serial.begin(MIDILAR_EXAMPLE_MIDI_BAUD);
    out.Start(12000);
}

void loop() {
    out.Update(Write);
}
