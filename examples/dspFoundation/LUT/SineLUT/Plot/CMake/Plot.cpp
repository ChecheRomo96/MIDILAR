#include <iostream>
#include <iomanip>
#include <string>
#include <MIDILAR_dspFoundation.h>

int main() {
    size_t LUT_SIZE = pow(2,11); // Ensure both arrays have the same size
    float INPUT_RANGE = LUT_SIZE; // Ensure both arrays have the same size
    constexpr size_t OVERSAMPLING = 2;
    constexpr size_t PERIODS = 1;

    MIDILAR::dspFoundation::LUT::Periodic::SineLUT<float, float> sineLUT;
    sineLUT.SetBufferSize(LUT_SIZE);
    sineLUT.SetInputRange(0, INPUT_RANGE);
    sineLUT.SetAmplitude(1.0f);
    sineLUT.SetDCOffset(0.0f);
    sineLUT.SetPhaseOffset(0.0f);

    constexpr float step = 1.0f / static_cast<float>(OVERSAMPLING);
    
    // Print MATLAB commands for lut_index
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "lut_index = [";
    for (size_t idx = 0; idx < PERIODS * (LUT_SIZE*OVERSAMPLING); ++idx) {
        float i = idx * step;
        std::cout << i;
        if (idx < (PERIODS * (LUT_SIZE*OVERSAMPLING))-1) std::cout << ", ";
    }
    std::cout << "];\n";

    // Print MATLAB commands for lut_value
    std::cout << "lut_value = [";
    for (size_t idx = 0; idx < PERIODS * (LUT_SIZE*OVERSAMPLING); ++idx) {
        float i = idx * step;
        std::cout << sineLUT.GetValue(i,false, true);
        if (idx < (PERIODS * (LUT_SIZE*OVERSAMPLING))-1) std::cout << ", ";
    }
    std::cout << "];\n";

    // Print MATLAB commands for lut_value with interpolation
    std::cout << "lut_value_interpolated = [";
    for (size_t idx = 0; idx < PERIODS * (LUT_SIZE*OVERSAMPLING); ++idx) {
        float i = idx * step;
        std::cout << sineLUT.GetValue(i, true, true);
        if (idx < (PERIODS * (LUT_SIZE*OVERSAMPLING))-1) std::cout << ", ";
    }
    std::cout << "];\n";

    // Improved MATLAB plot command
    std::cout << "theta = lut_index * 2 * pi * (1 / "<<std::to_string(LUT_SIZE)<<");\n";
    std::cout << "sine = sin(theta);\n";


    std::cout << "figure;\n";
    std::cout << "set(gcf, 'Position', [100, 100, 1200, 800]);\n"; // Adjust width to 1200px and height to 400px


    // First subplot: LUT vs. Interpolated LUT vs. Reference Sine
    std::cout << "subplot(1,3,1);\n";
    std::cout << "plot(lut_index, lut_value, lut_index, lut_value_interpolated, lut_index, sine);\n";
    std::cout << "xlabel('Index');\n";
    std::cout << "ylabel('LUT Value');\n";
    std::cout << "title('LUT vs Interpolation vs Sine');\n";
    std::cout << "legend('LUT', 'Interpolated LUT', 'Reference Sine');\n";
    std::cout << "grid on;\n";

    // Second subplot: Difference between Reference Sine and LUT
    std::cout << "subplot(1,3,2);\n";
    std::cout << "plot(lut_index, sine - lut_value);\n";
    std::cout << "xlabel('Index');\n";
    std::cout << "ylabel('Error');\n";
    std::cout << "title('Error: Sine - LUT');\n";
    std::cout << "grid on;\n";

    // Third subplot: Difference between Reference Sine and Interpolated LUT
    std::cout << "subplot(1,3,3);\n";
    std::cout << "plot(lut_index, sine - lut_value_interpolated);\n";
    std::cout << "xlabel('Index');\n";
    std::cout << "ylabel('Error');\n";
    std::cout << "title('Error: Sine - Interpolated LUT');\n";
    std::cout << "grid on;\n";

    std::cout << "annotation('textbox', [0.0, 0.9, 0.5, 0.1], 'String', 'LUT SIZE = 2^1^0', 'FontSize', 14, 'Rotation', 0, 'HorizontalAlignment', 'left','VerticalAlignment', 'top', 'EdgeColor', 'none');\n";

    return 0;
}
