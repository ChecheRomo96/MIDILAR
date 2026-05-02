#include <stdio.h>
#include <DspCore/LUT/LUT1D.h>

using namespace MIDILAR::DspCore::LUT;

int main() {
    LUT1D<float> lut(8);

    for (size_t i = 0; i < lut.Size(); ++i) {
        lut.SetBinValue(i, static_cast<float>(i));
    }

    printf("BasicFill example\n");

    for (size_t i = 0; i < lut.Size(); ++i) {
        printf("lut[%zu] = %f\n", i, lut.GetValue(i));
    }

    return 0;
}
