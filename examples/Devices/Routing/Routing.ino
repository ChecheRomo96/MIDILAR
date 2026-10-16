#include <MIDILAR_Devices.h>
#include "Shared.h"
void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    MIDILARExamples::Devices::Routing::Run([](const char* text) { Serial.print(text); });
}
void loop() {}
