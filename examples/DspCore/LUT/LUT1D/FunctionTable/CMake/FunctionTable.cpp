#include <stdio.h>
#include <DspCore/LUT/LUT1D.h>

using namespace MIDILAR::DspCore::LUT;

int main() {
    LUT1D<float> lut(16);

    for (size_t i = 0; i < lut.Size(); ++i) {
        float x = static_cast<float>(i) / static_cast<float>(lut.Size() - 1);
        lut.SetBinValue(i, x * x);
    }

    float input = 0.35f;
    size_t index = static_cast<size_t>(input * static_cast<float>(lut.Size() - 1));

    printf("FunctionTable example\n");
    printf("input = %f\n", input);
    printf("index = %zu\n", index);
    printf("approx x^2 = %f\n", lut.GetValue(index));

    return 0;
}
