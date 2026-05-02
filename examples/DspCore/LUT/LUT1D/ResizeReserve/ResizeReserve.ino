#include <MIDILAR.h>

using namespace MIDILAR::DspCore::LUT;

LUT1D<float> lut;

void setup() {
    Serial.begin(115200);

    Serial.println("ResizeReserve example");

    lut.Reserve(32);
    Serial.print("After Reserve(32): Size = ");
    Serial.print(lut.Size());
    Serial.print(", Capacity = ");
    Serial.println(lut.Capacity());

    lut.Resize(8);
    Serial.print("After Resize(8): Size = ");
    Serial.print(lut.Size());
    Serial.print(", Capacity = ");
    Serial.println(lut.Capacity());

    for (size_t i = 0; i < lut.Size(); ++i) {
        lut.SetBinValue(i, static_cast<float>(i * 10));
    }

    lut.Resize(16);
    Serial.print("After Resize(16): Size = ");
    Serial.print(lut.Size());
    Serial.print(", Capacity = ");
    Serial.println(lut.Capacity());

    for (size_t i = 0; i < lut.Size(); ++i) {
        Serial.print("lut[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(lut.GetValue(i));
    }
}

void loop() {}
