#include <stdio.h>
#include <dspFoundation/LUT/LUT1D.h>

using namespace MIDILAR::dspFoundation::LUT;

int main() {
    LUT1D<float> lut;

    printf("ResizeReserve example\n");

    lut.Reserve(32);
    printf("After Reserve(32): Size = %zu, Capacity = %zu\n", lut.Size(), lut.Capacity());

    lut.Resize(8);
    printf("After Resize(8): Size = %zu, Capacity = %zu\n", lut.Size(), lut.Capacity());

    for (size_t i = 0; i < lut.Size(); ++i) {
        lut.SetBinValue(i, static_cast<float>(i * 10));
    }

    lut.Resize(16);
    printf("After Resize(16): Size = %zu, Capacity = %zu\n", lut.Size(), lut.Capacity());

    for (size_t i = 0; i < lut.Size(); ++i) {
        printf("lut[%zu] = %f\n", i, lut.GetValue(i));
    }

    return 0;
}
