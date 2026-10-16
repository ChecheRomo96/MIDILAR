#include <MIDILAR_Protocol.h>
#include "Shared.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    MIDILARExamples::Protocol::UsbMidi1::Run([](const char* text) { Serial.print(text); });
}
void loop() {}
