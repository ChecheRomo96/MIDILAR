#include <iostream>
#include <fstream>
#include <string>
#include <dspFoundation/LUT/Shaping/GammaLUT.h>

using namespace MIDILAR::dspFoundation::LUT::Shaping;

int main() {
    const size_t width = 64;
    const size_t morphCount = 5;
    const float morphValues[morphCount] = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
    const float expGain = 10.0f;

    GammaLUT<uint8_t> lut;
    lut.ResizeLUT(width, 1, 1);  // Only 1 morph, 1 gain initially
    lut.SetGainRange(expGain);  // Fixed gain
    lut.SetOutputGain(UINT8_MAX);

    std::ofstream file("gamma_morph_luts.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file\n";
        return 1;
    }

    // Write one LUT per row
    for (size_t i = 0; i < morphCount; ++i) {
        lut.SetMorphRange(morphValues[i], morphValues[i]); // constant morph
        for (size_t x = 0; x < width; ++x) {
            file << static_cast<int>(lut.GetValue(x));
            if (x < width - 1) file << ",";
        }
        file << "\n";
        std::cout << "Written LUT for Morph = " << morphValues[i] << "\n";
    }

    file.close();
    std::cout << "Saved to gamma_morph_luts.csv\n";
    return 0;
}
