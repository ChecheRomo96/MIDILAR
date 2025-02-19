#include <iostream>
#include <vector>
#include <chrono>
#include <AudioFoundation/LUT/LogExpLUT.h>

constexpr size_t LUT_SIZE = UINT16_MAX; // LUT resolution
constexpr size_t NUM_ITERATIONS = 1000; // Number of recalculations to measure
constexpr float EXP_GAIN = 5.0f; // Fixed exponentiation gain factor

int main() {
    // Define morph values
    std::vector<float> MorphValues = { -1.0f, -0.8f, -0.6f, -0.4f, -0.2f, 0.0f, 
                                       0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

    // Create LogExpLUT instance
    MIDILAR::AudioFoundation::LUT::LogExpLUT<float, float> logExpLUT(LUT_SIZE);
    logExpLUT.SetInputRange(0, LUT_SIZE);
    logExpLUT.SetExponentiationGain(EXP_GAIN);
    logExpLUT.SetAmplitude(1.0f);
    logExpLUT.SetOffset(0.0f);

    // Timing variables
    auto start = std::chrono::high_resolution_clock::now();
    
    // Perform multiple recalculations
    for (size_t iter = 0; iter < NUM_ITERATIONS; ++iter) {
        for (float morph : MorphValues) {
            logExpLUT.SetMorph(morph); // This should trigger a recalculation
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Compute elapsed time
    std::chrono::duration<double, std::micro> elapsed = end - start;
    double avg_time_per_recalc = elapsed.count() / (NUM_ITERATIONS * MorphValues.size());

    // Print results
    std::cout << "Total time: " << elapsed.count() << " microseconds\n";
    std::cout << "Average time per recalculation: " << avg_time_per_recalc << " microseconds\n";

    return 0;
}
