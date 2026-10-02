// Sends MIDI clock at 120 BPM (Start, then 24 timing clocks per beat) and a
// Euclidean E(3, 8) rhythm on C2 on the Uno's serial port, for a MIDI shield.
//
// The Arduino builder only discovers libraries included from the sketch.
#include <Foundation.h>
#include <MCC.h>
#include <MIDILAR.h>

#include "Shared.h"

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
    Serial.begin(31250);
    out.Start(12000);
}

void loop() {
    out.Update(Write);
}
