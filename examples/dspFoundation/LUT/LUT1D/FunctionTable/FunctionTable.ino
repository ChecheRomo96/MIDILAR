#include <MIDILAR.h>

using namespace MIDILAR::dspFoundation::LUT;

LUT1D<float> lut(16);

void setup() {
    Serial.begin(115200);

    for (size_t i = 0; i < lut.Size(); ++i) {
        float x = static_cast<float>(i) / static_cast<float>(lut.Size() - 1);
        lut.SetBinValue(i, x * x);
    }

    float input = 0.35f;
    size_t index = static_cast<size_t>(input * static_cast<float>(lut.Size() - 1));

    Serial.println("FunctionTable example");
    Serial.print("input = ");
    Serial.println(input);
    Serial.print("index = ");
    Serial.println(index);
    Serial.print("approx x^2 = ");
    Serial.println(lut.GetValue(index));
}

void loop() {}
