// Sends MIDI Time Code at 25 fps from 01:00:00:00 on the Uno's serial port,
// for a MIDI shield: a full frame, then four quarter frames per frame.
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

MIDILARExamples::Devices::MtcOut::MtcOut out(Micros);

} // namespace

void setup() {
    Serial.begin(31250);
    out.Start(MIDILAR::Protocol::TimeCode::From(1, 0, 0, 0, MIDILAR::Protocol::TimeCodeRate::Fps25), Write);
}

void loop() {
    out.Update(Write);
}
