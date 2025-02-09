#include <Arduino.h>
#include <MIDILAR_AudioFoundation.h>

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for serial connection

    constexpr size_t LUT_SIZE = 128;
    constexpr size_t OVERSAMPLING = 10;

    MIDILAR::AudioFoundation::LUT::SineLUT<float, float> sineLUT(LUT_SIZE);
    sineLUT.SetInputRange(0, LUT_SIZE);
    sineLUT.SetAmplitude(1.0f);
    sineLUT.SetDCOffset(0.0f);
    sineLUT.SetPhaseOffset(0.0f);

    constexpr float step = 1.0f / static_cast<float>(OVERSAMPLING);
    
    Serial.print("lut_index = [");
    for (size_t idx = 0; idx < 2 * (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(i, 3);
        if (idx < (2 * (LUT_SIZE * OVERSAMPLING)) - 1) Serial.print(", ");
    }
    Serial.println("];");

    Serial.print("lut_value = [");
    for (size_t idx = 0; idx < 2 * (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(sineLUT.GetValue(i), 3);
        if (idx < (2 * (LUT_SIZE * OVERSAMPLING)) - 1) Serial.print(", ");
    }
    Serial.println("];");

    sineLUT.OutputInterpolation_Enable();
    
    Serial.print("lut_value_interpolated = [");
    for (size_t idx = 0; idx < 2 * (LUT_SIZE * OVERSAMPLING); ++idx) {
        float i = idx * step;
        Serial.print(sineLUT.GetValue(i), 3);
        if (idx < (2 * (LUT_SIZE * OVERSAMPLING)) - 1) Serial.print(", ");
    }
    Serial.println("];");

    Serial.print("theta = lut_index * 2 * pi * (1 / ");
    Serial.print(LUT_SIZE);
    Serial.println(");");
    Serial.println("sine = sin(theta);");

    Serial.println("figure;");
    Serial.println("set(gcf, 'Position', [100, 100, 1200, 800]);");
    
    Serial.println("subplot(1,3,1);");
    Serial.println("plot(lut_index, lut_value, lut_index, lut_value_interpolated, lut_index, sine);");
    Serial.println("xlabel('Index');");
    Serial.println("ylabel('LUT Value');");
    Serial.println("title('LUT vs Interpolation vs Sine');");
    Serial.println("legend('LUT', 'Interpolated LUT', 'Reference Sine');");
    Serial.println("grid on;");

    Serial.println("subplot(1,3,2);");
    Serial.println("plot(lut_index, sine - lut_value);");
    Serial.println("xlabel('Index');");
    Serial.println("ylabel('Error');");
    Serial.println("title('Error: Sine - LUT');");
    Serial.println("grid on;");

    Serial.println("subplot(1,3,3);");
    Serial.println("plot(lut_index, sine - lut_value_interpolated);");
    Serial.println("xlabel('Index');");
    Serial.println("ylabel('Error');");
    Serial.println("title('Error: Sine - Interpolated LUT');");
    Serial.println("grid on;");
}

void loop() {
    // Nothing to do in loop
}
