#include <iostream>
#include <cmath>
#include <dspFoundation/LUT/LUT2D/LUT2D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

using namespace MIDILAR::dspFoundation::LUT;

// ------------------------------------------------------------------------------------------------
// MorphingLUT: Generates a 2D LUT morphing between Sine and Square waves
// ------------------------------------------------------------------------------------------------

namespace{
template<typename T>
class MorphingLUT : public LUT2D<T> {
private:
    float amplitude;
    float dcOffset;

public:
    MorphingLUT(size_t width, size_t height, float amp, float dc)
        : amplitude(amp), dcOffset(dc)
    {
        this->SetBufferSize(width, height);
    }

protected:
    void Eval() override {
        size_t W = this->Width();   // LUT X dimension = phase steps
        size_t H = this->Height();  // LUT Y dimension = morph steps (0=sine, 1=square)

        T* buffer = this->GetBuffer();
        if (!buffer) return;

        T* tempSine = new T[W];
        T* tempSquare = new T[W];

        // Generate static waveforms
        MIDILAR::dspFoundation::Generators::Periodic::Sine<T>(tempSine, W, amplitude, dcOffset, 0.0f);
        MIDILAR::dspFoundation::Generators::Periodic::Square<T>(tempSquare, W, amplitude, dcOffset, 0.0f);

        for (size_t y = 0; y < H; ++y) {
            float morph = static_cast<float>(y) / (H - 1);  // from 0 (sine) to 1 (square)
            T* row = buffer + y * W;

            for (size_t x = 0; x < W; ++x) {
                row[x] = static_cast<T>((1.0f - morph) * tempSine[x] + morph * tempSquare[x]);
            }
        }

        delete[] tempSine;
        delete[] tempSquare;
    }
};
}

// ------------------------------------------------------------------------------------------------
// Main Program
// ------------------------------------------------------------------------------------------------

int main() {
    constexpr size_t WIDTH  = 32;   // LUT X dimension: phase steps
    constexpr size_t HEIGHT = 16;   // LUT Y dimension: morph steps

    MorphingLUT<float> lut(WIDTH, HEIGHT, 1.0f, 0.0f);  // Amplitude, DC offset

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

    // MATLAB plot code: separate figures
    std::cout << "figure;\n";
    std::cout << "imagesc(z); colormap('jet'); colorbar;\n";
    std::cout << "title('2D Morphing LUT (Sine to Square) - Imagesc');\n";
    std::cout << "xlabel('Phase'); ylabel('Morph');\n";

    std::cout << "figure;\n";
    std::cout << "surf(z); shading interp; colormap('jet'); colorbar;\n";
    std::cout << "title('2D Morphing LUT (Sine to Square) - Surface');\n";
    std::cout << "xlabel('Phase'); ylabel('Morph'); zlabel('Amplitude');\n";

    return 0;
}
