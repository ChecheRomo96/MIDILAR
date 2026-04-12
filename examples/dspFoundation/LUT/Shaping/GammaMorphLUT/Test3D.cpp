#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <dspFoundation/LUT/Shaping/GammaLUT.h>

using namespace MIDILAR::dspFoundation::LUT::Shaping;

int main() {
    const size_t width = 64;
    const size_t morphSteps = 64;
    const size_t gainSteps = 64;
    const float gainMin = 1.0f;
    const float gainMax = 10.0f;

    GammaLUT<float> lut;
    lut.ResizeLUT(width, morphSteps, gainSteps);
    lut.SetGainRange(gainMin, gainMax);
    lut.SetMorphRange(-1.0f, 1.0f);

    for (size_t k = 0; k < gainSteps; ++k) {
        std::ostringstream fname;
        fname << "GammaMorph_Gain_" << std::setfill('0') << std::setw(3) << k << ".csv";
        std::ofstream file(fname.str());

        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << fname.str() << "\n";
            continue;
        }

        for (size_t j = 0; j < morphSteps; ++j) {
            for (size_t i = 0; i < width; ++i) {
                file << lut.GetValue(i, j, k);
                if (i < width - 1) file << ",";
            }
            file << "\n";
        }

        file.close();
        std::cout << "Saved: " << fname.str() << "\n";
    }

    return 0;
}
