// A MIDI thru box on the Uno's serial port: notes are
// transposed up two semitones and kept on C major.

#include <MIDILAR_Devices.h>

#include "Shared.h"

// UART defaults to 115200 baud for a USB serial-to-MIDI bridge.
// For a DIN MIDI shield, compile with -DMIDILAR_EXAMPLE_MIDI_BAUD=31250.
// The stream contains MIDI bytes, not printable serial-monitor text.
#ifndef MIDILAR_EXAMPLE_MIDI_BAUD
    #define MIDILAR_EXAMPLE_MIDI_BAUD 115200
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
