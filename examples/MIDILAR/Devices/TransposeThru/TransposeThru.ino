// A MIDI thru box for a MIDI shield on the Uno's serial port: notes are
// transposed up two semitones and kept on C major.
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

MIDILARExamples::Devices::TransposeThru::Thru thru;

void setup() {
    Serial.begin(MIDILAR_EXAMPLE_MIDI_BAUD);
    thru.SetSemitones(2);
}

void loop() {
    while (Serial.available() > 0) {
        uint8_t out[MIDILARExamples::Devices::TransposeThru::Thru::Capacity];
        const uint8_t count = thru.Process(static_cast<uint8_t>(Serial.read()), out);
        Serial.write(out, count);
    }
}
