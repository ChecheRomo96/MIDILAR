#include <MIDILAR.h>

using namespace MIDILAR::DspCore::LUT;

LUT1D<float> CreateTable() {
    LUT1D<float> lut(4);

    lut.SetBinValue(0, 10.0f);
    lut.SetBinValue(1, 20.0f);
    lut.SetBinValue(2, 30.0f);
    lut.SetBinValue(3, 40.0f);

    return lut;
}

void setup() {
    Serial.begin(115200);

    Serial.println("MoveSemantics example");

    LUT1D<float> lut = CreateTable();

    for (size_t i = 0; i < lut.Size(); ++i) {
        Serial.print("lut[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(lut.GetValue(i));
    }

    LUT1D<float> movedLut;
    movedLut = static_cast<LUT1D<float>&&>(lut);

    Serial.println("Moved LUT:");

    for (size_t i = 0; i < movedLut.Size(); ++i) {
        Serial.print("movedLut[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(movedLut.GetValue(i));
    }

    Serial.print("Original after move: Size = ");
    Serial.print(lut.Size());
    Serial.print(", Capacity = ");
    Serial.println(lut.Capacity());
}

void loop() {}
