#include <math.h>
#include <MIDILAR_dspFoundation.h>  // Adjust path based on your project structure

using namespace MIDILAR::dspFoundation::LUT::Periodic;

void setup() {
    Serial.begin(115200);
    while (!Serial);  // Wait for Serial (for boards like Leonardo)

    constexpr size_t N = 32;

    // Instantiate and configure the SineLUT
    SineLUT<float> lut;
    lut.SetBufferSize(N);
    lut.SetAmplitude(1.0f, false);
    lut.SetDCOffset(0.0f, false);
    lut.SetPhaseOffset(0.0f, false);
    lut.Eval();  // Manually generate LUT after setting parameters

    // Print MATLAB-style code
    Serial.println(F("y = ["));
    for (size_t i = 0; i < N; ++i) {
        Serial.print(lut.GetValue(i), 6);
        if (i < N - 1) Serial.print(F(", "));
    }
    Serial.println(F("];"));

    Serial.print(F("x = linspace(0, 1, ")); Serial.print(N); Serial.println(F(");"));
    Serial.println(F("plot(x, y, '-o');"));
    Serial.println(F("title('Sine LUT');"));
    Serial.println(F("xlabel('Phase'); ylabel('Amplitude');"));
}

void loop() {
    // Nothing here — one-shot execution
}
