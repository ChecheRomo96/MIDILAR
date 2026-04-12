#include <iostream>
#include <cmath>
#include <dspFoundation/LUT/LUT2D/LUT2D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

using namespace MIDILAR::dspFoundation::LUT;

// ------------------------------------------------------------------------------------------------
// GeneratorLUT2D: Generates a 2D LUT with X = phase, Y = gain (amplitude)
// ------------------------------------------------------------------------------------------------
namespace{
template<typename T>
class GeneratorLUT2D : public LUT2D<T> {
private:
    float dcOffset;
    float gainMin, gainMax;
    float phaseMin, phaseMax;

public:
    GeneratorLUT2D(size_t width, size_t height, float dc, float gainStart, float gainEnd, float phaseStart, float phaseEnd)
        : dcOffset(dc), gainMin(gainStart), gainMax(gainEnd), phaseMin(phaseStart), phaseMax(phaseEnd)
    {
        this->SetBufferSize(width, height);
    }

protected:
    void Eval() override {
        size_t W = this->Width();   // LUT X dimension = phase steps
        size_t H = this->Height();  // LUT Y dimension = gain steps

        T* buffer = this->GetBuffer();
        if (!buffer) return;

        for (size_t y = 0; y < H; ++y) {
            float gain = gainMin + (gainMax - gainMin) * static_cast<float>(y) / (H - 1);
            float phaseOffset = phaseMin + (phaseMax - phaseMin) * static_cast<float>(y) / (H - 1);


            // Get pointer to start of row
            T* row = buffer + y * W;

            // Use generator to fill each row with the current gain
            MIDILAR::dspFoundation::Generators::Periodic::Sine<T>(
                row,        // write directly into the buffer
                W,          // buffer size per row (phase resolution)
                gain,       // amplitude
                dcOffset,   // DC offset
                phaseOffset // fixed phase offset (can be parametrized too)
            );
        }
    }   

};
}

// ------------------------------------------------------------------------------------------------
// Main Program
// ------------------------------------------------------------------------------------------------

int main() {
    constexpr size_t WIDTH  = 32;   // LUT X dimension: phase steps
    constexpr size_t HEIGHT = 16;   // LUT Y dimension: gain steps

    GeneratorLUT2D<float> lut(WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f);  // DC, gain range, phase range

    // Output MATLAB-style 2D array
    std::cout << "z = [ ...\n";
    for (size_t y = 0; y < HEIGHT; ++y) {
        std::cout << "  ";
        for (size_t x = 0; x < WIDTH; ++x) {
            std::cout << lut.GetValue(x, y);
            if (x < WIDTH - 1) std::cout << ", ";
        }
        std::cout << ";\n";
    }
    std::cout << "];\n";
    std::cout << "imagesc(z); colormap('jet'); colorbar;\n";
    std::cout << "title('2D Sine LUT (Gain vs Phase)');\n";
    std::cout << "xlabel('Phase'); ylabel('Gain');\n";

    return 0;
}
