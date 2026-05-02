#include <MIDILAR.h>

using namespace MIDILAR::DspCore::LUT;

float data[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };
LUT1D<float> lut;

void setup() {
    Serial.begin(115200);

    Serial.println("RawData example");

    if (!lut.SetRawData(data, 5)) {
        Serial.println("Failed to copy raw data.");
        return;
    }

    for (size_t i = 0; i < lut.Size(); ++i) {
        Serial.print("lut[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(lut.GetValue(i));
    }
}

void loop() {}
