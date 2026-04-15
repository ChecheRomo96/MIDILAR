#ifndef MIDILAR_PERIODIC_TRIANGLE_H
#define MIDILAR_PERIODIC_TRIANGLE_H

    #include <System/BuildSettings.h>

    #define _USE_MATH_DEFINES
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {
        template<typename OUTPUT_TYPE>
        void Triangle(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DCOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;

            float twoPi = static_cast<float>(2 * M_PI);
            float phaseStep = twoPi / static_cast<float>(BufferSize);

            // Apply a 270° (3π/2) phase shift to start at midpoint with rising slope
            float offsetRad = fmodf((PhaseOffset + 0.75f) * twoPi, twoPi);
            if (offsetRad < 0.0f) offsetRad += twoPi;

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = fmodf(i * phaseStep + offsetRad, twoPi);

                // Normalize phase to [0, 1] range
                float normalizedPhase = phase / twoPi;

                // Triangle wave calculation (rises to +1 then falls to -1)
                float triangleValue = 2.0f * fabsf(2.0f * normalizedPhase - 1.0f) - 1.0f;

                Buffer[i] = static_cast<OUTPUT_TYPE>(triangleValue * Amp + DCOffset);
            }
        }
    }

#endif//MIDILAR_PERIODIC_TRIANGLE_H
