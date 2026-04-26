#include <MIDILAR.h>

using namespace MIDILAR::dspFoundation::LUT;

LUT1D<float> lut(8);

void setup() {
    Serial.begin(115200);

    for (size_t i = 0; i < lut.Size(); ++i) {
        lut.SetBinValue(i, static_cast<float>(i));
    }

    Serial.println("BasicFill example");

    for (size_t i = 0; i < lut.Size(); ++i) {
        Serial.print("lut[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(lut.GetValue(i));
    }
}

void loop() {}
