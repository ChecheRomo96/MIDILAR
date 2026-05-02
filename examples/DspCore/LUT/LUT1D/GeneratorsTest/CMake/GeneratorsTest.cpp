#include <iostream>
#include <cmath>

#include <DspCore/Generators.h>
#include <DspCore/LUT/LUT1D.h>

using namespace MIDILAR::DspCore::Generators::Periodic;
using namespace MIDILAR::DspCore::Generators::Shaping;
using namespace MIDILAR::DspCore::LUT;

#define SIZE 24

void PrintBuffer(const char* name, float* buffer) {
    std::cout << name << ":\n";
    for (size_t i = 0; i < SIZE; ++i) {
        std::cout << buffer[i] << " ";
    }
    std::cout << "\n\n";
}

int main() {
    
    LUT1D<float> lut(SIZE);

    std::cout << "=== Generator Tests ===\n\n";

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_SINE
        std::cout << "Testing Sine Generator...\n";
        // SINE
        SineLUT<float>(lut, 1.0f, 0.0f, 0.0f);
        PrintBuffer("Sine", lut.GetBuffer());
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_SQUARE
        std::cout << "Testing Square Generator...\n";
        // SQUARE
        SquareLUT<float>(lut, 1.0f, 0.0f, 0.0f);
        PrintBuffer("Square", lut.GetBuffer());
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_TRIANGLE
        std::cout << "Testing Triangle Generator...\n";
        // TRIANGLE
        TriangleLUT<float>(lut, 1.0f, 0.0f, 0.0f);
        PrintBuffer("Triangle", lut.GetBuffer());
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_SAW
        std::cout << "Testing Saw Generator...\n";
        // SAW
        SawLUT<float>(lut, 1.0f, 0.0f, 0.0f);
        PrintBuffer("Saw", lut.GetBuffer());
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_SHAPING
        std::cout << "Testing Gamma Generator...\n";
        // GAMMA
        GammaLUT<float>(lut, 2.0f, 1.0f);
        PrintBuffer("Gamma", lut.GetBuffer());
    #endif

    return 0;
}