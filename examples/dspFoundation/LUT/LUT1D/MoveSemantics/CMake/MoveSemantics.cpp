#include <stdio.h>
#include <dspFoundation/LUT/LUT1D.h>

using namespace MIDILAR::dspFoundation::LUT;

static LUT1D<float> CreateTable() {
    LUT1D<float> lut(4);

    lut.SetBinValue(0, 10.0f);
    lut.SetBinValue(1, 20.0f);
    lut.SetBinValue(2, 30.0f);
    lut.SetBinValue(3, 40.0f);

    return lut;
}

int main() {
    printf("MoveSemantics example\n");

    LUT1D<float> lut = CreateTable();

    for (size_t i = 0; i < lut.Size(); ++i) {
        printf("lut[%zu] = %f\n", i, lut.GetValue(i));
    }

    LUT1D<float> movedLut;
    movedLut = static_cast<LUT1D<float>&&>(lut);

    printf("\nMoved LUT:\n");

    for (size_t i = 0; i < movedLut.Size(); ++i) {
        printf("movedLut[%zu] = %f\n", i, movedLut.GetValue(i));
    }

    printf("\nOriginal after move: Size = %zu, Capacity = %zu\n", lut.Size(), lut.Capacity());

    return 0;
}
