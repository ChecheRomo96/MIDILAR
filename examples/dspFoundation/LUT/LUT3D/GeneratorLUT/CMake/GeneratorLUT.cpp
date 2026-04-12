#include <iostream>
#include <cmath>
#include <limits>
#include <dspFoundation/LUT/LUT3D/LUT3D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

using namespace MIDILAR::dspFoundation::LUT;

// ------------------------------------------------------------------------------------------------
// GeneratorLUT3D: Generates a 3D LUT with X = phase, Y = gain (amplitude), Z = DC offset
// ------------------------------------------------------------------------------------------------

namespace{
template<typename T>
class GeneratorLUT3D : public LUT3D<T> {
private:
    float gainMin, gainMax;
    float phaseMin, phaseMax;
    float dcMin, dcMax;

public:
    GeneratorLUT3D(size_t width, size_t height, size_t depth,
                   float gainStart, float gainEnd,
                   float phaseStart, float phaseEnd,
                   float dcStart, float dcEnd)
        : gainMin(gainStart), gainMax(gainEnd),
          phaseMin(phaseStart), phaseMax(phaseEnd),
          dcMin(dcStart), dcMax(dcEnd)
    {
        this->SetBufferSize(width, height, depth);
    }

protected:
    void Eval() override {
        size_t W = this->Width();   // Phase resolution (X)
        size_t H = this->Height();  // Gain resolution (Y)
        size_t D = this->Depth();   // DC offset resolution (Z)

        T* buffer = this->GetBuffer();
        if (!buffer) return;

        for (size_t z = 0; z < D; ++z) {
            float dc = dcMin + (dcMax - dcMin) * static_cast<float>(z) / (D - 1);
            for (size_t y = 0; y < H; ++y) {
                float gain = gainMin + (gainMax - gainMin) * static_cast<float>(y) / (H - 1);
                float phaseOffset = phaseMin + (phaseMax - phaseMin) * static_cast<float>(y) / (H - 1);

                T* slice = buffer + (z * H + y) * W; // Get pointer to start of row

                MIDILAR::dspFoundation::Generators::Periodic::Sine<T>(
                    slice,
                    W,
                    gain,
                    dc,
                    phaseOffset
                );
            }
        }
    }
};
}

// ------------------------------------------------------------------------------------------------
// Main Program
// ------------------------------------------------------------------------------------------------

int main() {
    constexpr size_t WIDTH  = 32;   // Phase
    constexpr size_t HEIGHT = 16;   // Gain
    constexpr size_t DEPTH  = 16;    // DC Offset

    GeneratorLUT3D<float> lut(WIDTH, HEIGHT, DEPTH, -1.0f, 1.0f, 0.0f, 1.0f, -0.5f, 0.5f);

    float globalMin = std::numeric_limits<float>::max();
    float globalMax = std::numeric_limits<float>::lowest();

    // Precompute global min/max
    for (size_t z = 0; z < DEPTH; ++z) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            for (size_t x = 0; x < WIDTH; ++x) {
                float val = lut.GetValue(x, y, z);
                if (val < globalMin) globalMin = val;
                if (val > globalMax) globalMax = val;
            }
        }
    }

    // Print slices and plot commands
    for (size_t z = 0; z < DEPTH; ++z) {
        std::cout << "z_slice_" << z << " = [ ...\n";
        for (size_t y = 0; y < HEIGHT; ++y) {
            std::cout << "  ";
            for (size_t x = 0; x < WIDTH; ++x) {
                std::cout << lut.GetValue(x, y, z);
                if (x < WIDTH - 1) std::cout << ", ";
            }
            std::cout << ";\n";
        }
        std::cout << "];\n";
        std::cout << "figure;\nimagesc(z_slice_" << z << "); colormap('jet'); colorbar;\n";
        std::cout << "caxis([" << globalMin << ", " << globalMax << "]);\n";
        std::cout << "title('3D LUT Slice - DC Offset Layer " << z << "');\n";
        std::cout << "xlabel('Phase'); ylabel('Gain');\n";
    }


    return 0;
}
