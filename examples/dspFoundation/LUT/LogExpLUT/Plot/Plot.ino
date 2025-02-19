#include <Arduino.h>
#include <MIDILAR_AudioFoundation.h>

constexpr size_t LUT_SIZE = 128; // LUT resolution
constexpr size_t OVERSAMPLING = 2; // Number of interpolated points per LUT step

// Define morph values
constexpr float MorphValues[] = { -1.0f, -0.8f, -0.6f, -0.4f, -0.2f, 0.0f, 
                                  0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

constexpr float EXP_GAIN = 5.0f; // Fixed exponentiation gain factor
constexpr float step = 1.0f / static_cast<float>(OVERSAMPLING);

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor

    // Print MATLAB commands for lut_index
    Serial.print("lut_index = [");
    for (size_t idx = 0; idx < (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(i, 3);
        if (idx < (LUT_SIZE * OVERSAMPLING) - 1) Serial.print(", ");
    }
    Serial.print("];");

    // Create LogExpLUT instance
    MIDILAR::AudioFoundation::LUT::LogExpLUT<float, float> logExpLUT(LUT_SIZE);
    logExpLUT.SetInputRange(0, LUT_SIZE);
    logExpLUT.SetExponentiationGain(EXP_GAIN);
    logExpLUT.SetAmplitude(1.0f);
    logExpLUT.SetOffset(0.0f);
    logExpLUT.OutputInterpolation_Enable();

    // Print MATLAB commands for multiple morph curves
    for (size_t mIdx = 0; mIdx < (sizeof(MorphValues) / sizeof(MorphValues[0])); ++mIdx) {
        float morph = MorphValues[mIdx];
        logExpLUT.SetMorph(morph);

        Serial.print("lut_value_m");
        Serial.print(mIdx);
        Serial.print(" = [");

        for (size_t idx = 0; idx < (LUT_SIZE * OVERSAMPLING); ++idx) {
            float i = idx * step;
            Serial.print(logExpLUT.GetValue(i), 6);
            if (idx < (LUT_SIZE * OVERSAMPLING) - 1) Serial.print(", ");
        }
        Serial.println("];");
    }

    // MATLAB Plot Commands
    Serial.println("figure;");
    Serial.println("set(gcf, 'Position', [100, 100, 800, 800]);"); // Set window size

    Serial.println("hold on;");
    for (size_t mIdx = 0; mIdx < (sizeof(MorphValues) / sizeof(MorphValues[0])); ++mIdx) {
        Serial.print("plot(lut_index, lut_value_m");
        Serial.print(mIdx);
        Serial.println(");");
    }
    Serial.println("hold off;");

    Serial.println("xlabel('Index');");
    Serial.println("ylabel('LUT Value');");
    Serial.println("title('LogExpLUT for Different Morph Values');");
    Serial.println("legend('m=-1','m=-0.8','m=-0.6','m=-0.4','m=-0.2','m=0','m=0.2','m=0.4','m=0.6','m=0.8','m=1');");
    Serial.print("axis([0 ");
    Serial.print(LUT_SIZE);
    Serial.println(" 0 1]);"); // Set X-axis and Y-axis ranges to [0,1]
    Serial.println("grid on;");
}

void loop() {
    // Nothing to do in the loop
}
