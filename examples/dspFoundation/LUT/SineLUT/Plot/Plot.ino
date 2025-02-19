#include <Arduino.h>
#include <MIDILAR_dspFoundation.h>

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor to open

    const size_t LUT_SIZE = 1 << 10;  // Reduce to 2^10 = 1024 for Arduino memory safety
    const float INPUT_RANGE = LUT_SIZE;
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
    Serial.print("lut_index = [");
    for (size_t idx = 0; idx < PERIODS * (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(i, 3);
        if (idx < (PERIODS * (LUT_SIZE * OVERSAMPLING)) - 1) Serial.print(", ");
    }
    Serial.println("];");

    // Print MATLAB commands for lut_value
    Serial.print("lut_value = [");
    for (size_t idx = 0; idx < PERIODS * (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(sineLUT.GetValue(i, false, true), 6);
        if (idx < (PERIODS * (LUT_SIZE * OVERSAMPLING)) - 1) Serial.print(", ");
    }
    Serial.println("];");

    // Print MATLAB commands for lut_value with interpolation
    Serial.print("lut_value_interpolated = [");
    for (size_t idx = 0; idx < PERIODS * (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(sineLUT.GetValue(i, true, true), 6);
        if (idx < (PERIODS * (LUT_SIZE * OVERSAMPLING)) - 1) Serial.print(", ");
    }
    Serial.println("];");

    // Compute and print MATLAB sine reference
    Serial.print("theta = lut_index * 2 * pi * (1 / ");
    Serial.print(LUT_SIZE);
    Serial.println(");\n");

    Serial.println("sine = sin(theta);");

    // MATLAB Plot Setup
    Serial.println("figure;");
    Serial.println("set(gcf, 'Position', [100, 100, 1200, 800]);");

    // First subplot: LUT vs. Interpolated LUT vs. Reference Sine
    Serial.println("subplot(1,3,1);");
    Serial.println("plot(lut_index, lut_value, lut_index, lut_value_interpolated, lut_index, sine);");
    Serial.println("xlabel('Index');");
    Serial.println("ylabel('LUT Value');");
    Serial.println("title('LUT vs Interpolation vs Sine');");
    Serial.println("legend('LUT', 'Interpolated LUT', 'Reference Sine');");
    Serial.println("grid on;");

    // Second subplot: Difference between Reference Sine and LUT
    Serial.println("subplot(1,3,2);");
    Serial.println("plot(lut_index, sine - lut_value);");
    Serial.println("xlabel('Index');");
    Serial.println("ylabel('Error');");
    Serial.println("title('Error: Sine - LUT');");
    Serial.println("grid on;");

    // Third subplot: Difference between Reference Sine and Interpolated LUT
    Serial.println("subplot(1,3,3);");
    Serial.println("plot(lut_index, sine - lut_value_interpolated);");
    Serial.println("xlabel('Index');");
    Serial.println("ylabel('Error');");
    Serial.println("title('Error: Sine - Interpolated LUT');");
    Serial.println("grid on;");

    // Annotation
    Serial.println("annotation('textbox', [0.0, 0.9, 0.5, 0.1], 'String', 'LUT SIZE = 2^{10}', 'FontSize', 14, 'Rotation', 0, 'HorizontalAlignment', 'left','VerticalAlignment', 'top', 'EdgeColor', 'none');");
}

void loop() {
    // Nothing needed in loop since it's a one-time data print
}
