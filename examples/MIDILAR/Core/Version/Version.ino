// The Arduino builder only discovers libraries included from the sketch.
#include <Foundation.h>
#include <MCC.h>
#include <MIDILAR.h>

#include "Shared.h"

void setup() {
    Serial.begin(115200);
    while(!Serial) {}

    Serial.println("MIDILAR :: Core / Version");
    Serial.print("MIDILAR ....... ");
    Serial.println(MIDILARExamples::Core::Version::MIDILARVersion());
    Serial.print("MCC ........... ");
    Serial.println(MIDILARExamples::Core::Version::MCCVersion());
    Serial.print("Foundation .... ");
    Serial.println(MIDILARExamples::Core::Version::FoundationVersion());
}

void loop() {}
