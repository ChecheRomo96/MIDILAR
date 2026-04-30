#ifndef MIDILAR_DSPFOUNDATION_GENERATORS_NOISE_WHITENOISE_H
#define MIDILAR_DSPFOUNDATION_GENERATORS_NOISE_WHITENOISE_H

#include <MIDILAR_BuildSettings.h>
#include <stddef.h>
#include <stdint.h>

namespace MIDILAR::dspFoundation::Generators::Noise {

    /**
     * @brief Generates white noise in the range [-Amp, +Amp] + DcOffset.
     *
     * @tparam T Output buffer data type
     * @param Buffer Output buffer
     * @param TotalSamples Total number of samples in the buffer
     * @param Amp Peak amplitude of the noise
     * @param DcOffset DC offset added to the signal
     * @param Seed Pseudo-random generator seed/state
     */
    template<typename T>
    void White(
        T* Buffer,
        size_t TotalSamples,
        float Amp = 1.0f,
        float DcOffset = 0.0f,
        uint32_t* Seed = nullptr
    ) {
        // Validate parameters
        if (!Buffer) return;
        if (TotalSamples == 0) return;

        // Local seed if none provided
        uint32_t LocalSeed = 0x12345678u;
        uint32_t& State = Seed ? *Seed : LocalSeed;

        for (size_t i = 0; i < TotalSamples; i++) {

            // xorshift32
            State ^= State << 13;
            State ^= State >> 17;
            State ^= State << 5;

            // Convert to [0,1]
            float Random01 = static_cast<float>(State) * (1.0f / 4294967295.0f);

            // Convert to [-1,1]
            float RandomSigned = (Random01 * 2.0f) - 1.0f;

            // Apply amplitude and offset
            float Value = (RandomSigned * Amp) + DcOffset;

            Buffer[i] = static_cast<T>(Value);
        }
    }

} // namespace MIDILAR::dspFoundation::Generators::Noise

#endif // MIDILAR_DSPFOUNDATION_GENERATORS_NOISE_WHITENOISE_H
