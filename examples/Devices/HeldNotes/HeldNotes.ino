#include <MIDILAR_Devices.h>
#include "Shared.h"
void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    MIDILARExamples::Devices::HeldNotes::Run([](const char* text) { Serial.print(text); });
}
void loop() {}
