// Sends MIDI clock at 120 BPM (Start, then 24 timing clocks per beat) and a
// Euclidean E(3, 8) rhythm on C2 on the Uno's serial port, for a MIDI shield.
//
// The Arduino builder only discovers libraries included from the sketch.
#include <Foundation.h>
#include <MCC.h>
#include <MIDILAR.h>

#include "Shared.h"

// 31250 baud drives a MIDI shield. To test over the USB cable with a
// serial-to-MIDI bridge (Hairless MIDI) instead, compile with
// -DMIDILAR_EXAMPLE_MIDI_BAUD=115200.
#ifndef MIDILAR_EXAMPLE_MIDI_BAUD
    #define MIDILAR_EXAMPLE_MIDI_BAUD 31250
#endif

namespace {

uint32_t Micros() {
    return micros();
}

void Write(uint8_t byte) {
    Serial.write(byte);
}

MIDILARExamples::Devices::ClockOut::ClockOut out(Micros);

} // namespace

void setup() {
    Serial.begin(MIDILAR_EXAMPLE_MIDI_BAUD);
    out.Start(12000);
}

void loop() {
    out.Update(Write);
}
