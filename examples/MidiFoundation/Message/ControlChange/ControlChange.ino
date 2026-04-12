#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiFoundation;

void PrintHex(const char* label, const uint8_t* data, size_t size) {
    Serial.print(label);
    Serial.print(" ");
    for (size_t i = 0; i < size; ++i) {
        if (data[i] < 0x10) Serial.print("0");
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Message msg;

    // Bank Select
    msg.CC_BankSelect(1, 0);
    PrintHex("[Bank Select]", msg.Buffer(), msg.size());

    // Modulation
    msg.CC_Modulation(64, 0);
    PrintHex("[Modulation]", msg.Buffer(), msg.size());

    // Breath Control
    msg.CC_BreathControl(32, 0);
    PrintHex("[Breath Control]", msg.Buffer(), msg.size());

    // Volume
    msg.CC_Volume(100, 0);
    PrintHex("[Volume]", msg.Buffer(), msg.size());

    // Pan
    msg.CC_Pan(64, 0);
    PrintHex("[Pan Center]", msg.Buffer(), msg.size());

    // Expression
    msg.CC_Expression(80, 0);
    PrintHex("[Expression]", msg.Buffer(), msg.size());

    // All Sound Off
    msg.CC_AllSoundOff(0);
    PrintHex("[All Sound Off]", msg.Buffer(), msg.size());

    // All Notes Off
    msg.CC_AllNotesOff(0);
    PrintHex("[All Notes Off]", msg.Buffer(), msg.size());
}

void loop() {
    // Nothing here
}
