#include <iostream>
#include <iomanip>
#include <vector>
#include <MIDILAR_AudioFoundation.h>

int main() {
    constexpr size_t LUT_SIZE = 128; // LUT resolution
    constexpr size_t OVERSAMPLING = 2; // Number of interpolated points per LUT step

    // Define morph values
    std::vector<float> MorphValues = { -1.0f, -0.8f, -0.6f, -0.4f, -0.2f, 0.0f, 
                                        0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

    constexpr float EXP_GAIN = 5.0f; // Fixed exponentiation gain factor
    constexpr float step = 1.0f / static_cast<float>(OVERSAMPLING);
    
    // Print MATLAB commands for lut_index
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "lut_index = [";
    for (size_t idx = 0; idx < (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        std::cout << i;
        if (idx < (LUT_SIZE * OVERSAMPLING) - 1) std::cout << ", ";
    }
    std::cout << "];\n";

    // Create LogExpLUT instance
    MIDILAR::AudioFoundation::LUT::LogExpLUT<float, float> logExpLUT(LUT_SIZE);
    logExpLUT.SetInputRange(0, LUT_SIZE);
    logExpLUT.SetExponentiationGain(EXP_GAIN);
    logExpLUT.SetAmplitude(1.0f);
    logExpLUT.SetOffset(0.0f);

    // Print MATLAB commands for multiple morph curves
    for (size_t mIdx = 0; mIdx < MorphValues.size(); ++mIdx) {
        float morph = MorphValues[mIdx];
        logExpLUT.SetMorph(morph);

        std::cout << "lut_value_m" << mIdx << " = [";
        for (size_t idx = 0; idx < (LUT_SIZE * OVERSAMPLING); ++idx) {
            float i = idx * step;
            std::cout << logExpLUT.GetValue(i);
            if (idx < (LUT_SIZE * OVERSAMPLING) - 1) std::cout << ", ";
        }
        std::cout << "];\n";
    }

    // MATLAB Plot Commands
    std::cout << "figure;\n";
    std::cout << "set(gcf, 'Position', [100, 100, 800, 800]);\n"; // Set window size

    std::cout << "hold on;\n";
    for (size_t mIdx = 0; mIdx < MorphValues.size(); ++mIdx) {
        std::cout << "plot(lut_index, lut_value_m" << mIdx << ");\n";
    }
    std::cout << "hold off;\n";

    std::cout << "xlabel('Index');\n";
    std::cout << "ylabel('LUT Value');\n";
    std::cout << "title('LogExpLUT for Different Morph Values');\n";
    std::cout << "legend('m=-1','m=-0.8','m=-0.6','m=-0.4','m=-0.2','m=0','m=0.2','m=0.4','m=0.6','m=0.8','m=1');\n";
    std::cout << "axis([0 "<<std::to_string(LUT_SIZE)<<" 0 1]);\n"; // Set X-axis and Y-axis ranges to [0,1]
    std::cout << "grid on;\n";

    return 0;
}
