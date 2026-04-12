#include <iostream>
#include <cmath>
#include <MIDILAR_dspFoundation.h>

using namespace MIDILAR::dspFoundation::LUT::Periodic;

int main() {
    constexpr size_t N = 32;

    // Create and configure SineLUT
    SineLUT<float> lut;
    lut.SetBufferSize(N);
    lut.SetAmplitude(1.0f, false);
    lut.SetDCOffset(0.0f, false);
    lut.SetPhaseOffset(0.0f, false);

    // Output MATLAB-friendly array
    std::cout << "y = [";
    for (size_t i = 0; i < N; ++i) {
        std::cout << lut.GetValue(i);
        if (i < N - 1) std::cout << ", ";
    }
    std::cout << "];\n";

    std::cout << "x = linspace(0, 1, " << N << ");\n";
    std::cout << "plot(x, y, '-o');\n";
    std::cout << "title('Sine LUT');\n";
    std::cout << "xlabel('Phase'); ylabel('Amplitude');\n";

    return 0;
}
