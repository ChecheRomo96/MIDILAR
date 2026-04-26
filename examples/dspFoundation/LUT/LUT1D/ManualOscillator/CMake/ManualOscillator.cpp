#include <stdio.h>
#include <MIDILAR/dspFoundation/LUT/LUT1D/LUT1D.h>

using namespace MIDILAR::dspFoundation::LUT;

int main() {
    LUT1D<float> waveform(4);

    waveform.SetBinValue(0,  0.0f);
    waveform.SetBinValue(1,  1.0f);
    waveform.SetBinValue(2,  0.0f);
    waveform.SetBinValue(3, -1.0f);

    float phase = 0.0f;
    float phaseIncrement = 0.125f;

    printf("ManualOscillator example\n");

    for (size_t n = 0; n < 16; ++n) {
        size_t index = static_cast<size_t>(phase * static_cast<float>(waveform.Size()));

        if (index >= waveform.Size()) {
            index = waveform.Size() - 1;
        }

        float sample = waveform.GetValue(index);

        printf("n = %zu, phase = %f, index = %zu, sample = %f\n",
               n, phase, index, sample);

        phase += phaseIncrement;

        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
    }

    return 0;
}
