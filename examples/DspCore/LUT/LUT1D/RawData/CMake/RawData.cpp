#include <stdio.h>
#include <DspCore/LUT/LUT1D.h>

using namespace MIDILAR::DspCore::LUT;

int main() {
    float data[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };

    LUT1D<float> lut;

    printf("RawData example\n");

    if (!lut.SetRawData(data, 5)) {
        printf("Failed to copy raw data.\n");
        return 1;
    }

    for (size_t i = 0; i < lut.Size(); ++i) {
        printf("lut[%zu] = %f\n", i, lut.GetValue(i));
    }

    return 0;
}
