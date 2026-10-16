#include <MIDILAR_Core.h>

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
