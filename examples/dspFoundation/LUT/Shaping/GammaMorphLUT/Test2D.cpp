#include <iostream>
#include <fstream>
#include <string>
#include <dspFoundation/LUT/Shaping/GammaLUT.h>

using namespace MIDILAR::dspFoundation::LUT::Shaping;

int main() {
    const size_t width = 64;
    const size_t morphSteps = 65;
    const float expGain = 10.0f;

    GammaLUT<float> lut;
    lut.ResizeLUT(width, morphSteps, 1); // 2D LUT: Width x Morph
    lut.SetGainRange(expGain);           // fixed gain
    lut.SetMorphRange(-1.0f, 1.0f);      // variable morph range

    std::ofstream file("gamma_morph_2d.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    for (size_t j = 0; j < morphSteps; ++j) {
        for (size_t i = 0; i < width; ++i) {
            float value = lut.GetValue(i, j); // GetValue(x, y)
            file << value;
            if (i < width - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Saved: gamma_morph_2d.csv\n";
    return 0;
}
