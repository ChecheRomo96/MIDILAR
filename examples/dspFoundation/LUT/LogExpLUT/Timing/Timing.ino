#include <Arduino.h>
#include <AudioFoundation/LUT/LogExpLUT.h>

constexpr size_t LUT_SIZE = 512; // Reduced to fit Arduino memory
constexpr size_t NUM_ITERATIONS = 100; // Adjusted to prevent long execution time
constexpr float EXP_GAIN = 5.0f; // Fixed exponentiation gain factor

// Define morph values
constexpr float MorphValues[] = { -1.0f, -0.8f, -0.6f, -0.4f, -0.2f, 0.0f, 
                                  0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor

    Serial.println("Starting LogExpLUT Benchmark...");

    // Create LogExpLUT instance
    MIDILAR::AudioFoundation::LUT::LogExpLUT<float, float> logExpLUT(LUT_SIZE);
    logExpLUT.SetInputRange(0, LUT_SIZE);
    logExpLUT.SetExponentiationGain(EXP_GAIN);
    logExpLUT.SetAmplitude(1.0f);
    logExpLUT.SetOffset(0.0f);
    logExpLUT.OutputInterpolation_Enable();

    unsigned long startTime = micros();

    // Perform multiple recalculations
    for (size_t iter = 0; iter < NUM_ITERATIONS; ++iter) {
        for (size_t mIdx = 0; mIdx < (sizeof(MorphValues) / sizeof(MorphValues[0])); ++mIdx) {
            logExpLUT.SetMorph(MorphValues[mIdx]); // This should trigger a recalculation
        }
    }

    unsigned long endTime = micros();
    
    // Compute elapsed time
    unsigned long totalTime = endTime - startTime;
    float avgTimePerRecalc = totalTime / float(NUM_ITERATIONS * (sizeof(MorphValues) / sizeof(MorphValues[0])));

    // Print results
    Serial.print("Total time: ");
    Serial.print(totalTime);
    Serial.println(" microseconds");

    Serial.print("Average time per recalculation: ");
    Serial.print(avgTimePerRecalc, 3);
    Serial.println(" microseconds");

    Serial.println("Benchmark Completed.");
}

void loop() {
    // Nothing to do in loop
}
