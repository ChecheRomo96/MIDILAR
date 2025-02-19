#include <Arduino.h>
#include <MIDILAR_AudioFoundation.h>

using namespace MIDILAR::AudioFoundation::LUT2D;

constexpr size_t LUT_SIZE_X = 128;  // LUT resolution for velocity input
constexpr size_t LUT_SIZE_Y = 11;   // LUT resolution for morph values (matching MorphValues array)
constexpr float EXP_GAIN = 5.0;    // Fixed exponentiation gain factor

// Define morph values (Y dimension)
constexpr float MorphValues[LUT_SIZE_Y] = { -1.0f, -0.8f, -0.6f, -0.4f, -0.2f, 0.0f, 
                                            0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor

    // Print MATLAB commands for input (X-axis)
    Serial.print("lut_x = linspace(0, 1, ");
    Serial.print(LUT_SIZE_X);
    Serial.println(");"); 

    // Print MATLAB commands for morph values (Y-axis)
    Serial.print("lut_y = [");
    for (size_t j = 0; j < LUT_SIZE_Y; ++j) {
        Serial.print(MorphValues[j], 2);
        if (j < LUT_SIZE_Y - 1) Serial.print(", ");
    }
    Serial.println("];");

    // Create LogExpLUT2D instance
    LogExpLUT<float, float> logExpLUT(LUT_SIZE_X, LUT_SIZE_Y);
    logExpLUT.SetInputRange(0, 1, -1.0f, 1.0f); // X from [0,1], Y (morph) from [-1,1]
    logExpLUT.SetExponentiationGain(EXP_GAIN);
    logExpLUT.SetAmplitude(1.0f);
    logExpLUT.SetOffset(0.0f);
    //logExpLUT.OutputInterpolation_Enable();

    // Print MATLAB matrix for Z-values
    Serial.print("lut_z = [");
    for (size_t j = 0; j < LUT_SIZE_Y; ++j) {
        for (size_t i = 0; i < LUT_SIZE_X; ++i) {
            float x = (float)i / (LUT_SIZE_X - 1); // Normalize X in range [0,1]
            float y = MorphValues[j]; // Use predefined morph values
            Serial.print(logExpLUT.GetValue(x, y), 6);
            if (i < LUT_SIZE_X - 1) Serial.print(", ");
        }
        if (j < LUT_SIZE_Y - 1) Serial.print(";"); // Separate rows in MATLAB
    }
    Serial.println("];"); // Close matrix

    // MATLAB Surface Plot Commands
    Serial.println("figure;");
    Serial.println("imagesc(lut_x, lut_y, lut_z);");  // Top-down heatmap
    Serial.println("colormap(jet);");  // Use jet colormap for better contrast
    Serial.println("colorbar;");  // Add color reference
    Serial.println("xlabel('Input Velocity');");
    Serial.println("ylabel('Morph');");
    Serial.println("title('LogExpLUT2D Output Surface');");
    Serial.println("axis xy;");  // Ensure Y-axis is oriented correctly
    Serial.println("grid on;");
}

void loop() {
    // Nothing to do in the loop
}
