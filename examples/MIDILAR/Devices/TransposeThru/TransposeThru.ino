// A MIDI thru box for a MIDI shield on the Uno's serial port: notes are
// transposed up two semitones and kept on C major.
//
// The Arduino builder only discovers libraries included from the sketch.
#include <Foundation.h>
#include <MCC.h>
#include <MIDILAR.h>

#include "Shared.h"

MIDILARExamples::Devices::TransposeThru::Thru thru;

void setup() {
    Serial.begin(31250);
    thru.SetSemitones(2);
}

void loop() {
    while (Serial.available() > 0) {
        uint8_t out[MIDILARExamples::Devices::TransposeThru::Thru::Capacity];
        const uint8_t count = thru.Process(static_cast<uint8_t>(Serial.read()), out);
        Serial.write(out, count);
    }
}
