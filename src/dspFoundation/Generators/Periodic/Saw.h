#ifndef MIDILAR_PERIODIC_SAW_H
#define MIDILAR_PERIODIC_SAW_H

    #include <MIDILAR_BuildSettings.h>

    #define _USE_MATH_DEFINES
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {
        template<typename OUTPUT_TYPE> 
        void Saw(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DCOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;

            float twoPi = static_cast<float>(2 * M_PI);
            float phaseStep = twoPi / static_cast<float>(BufferSize);

            // Apply π phase shift so signal starts at 0 (midpoint)
            float offsetRad = fmodf((PhaseOffset + 0.5f) * twoPi, twoPi);  // 0.5 cycle = π
            if (offsetRad < 0.0f) offsetRad += twoPi;

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = fmodf(i * phaseStep + offsetRad, twoPi);
                float value = ((phase / static_cast<float>(M_PI)) - 1.0f);  // Range: [-1, +1]
                Buffer[i] = static_cast<OUTPUT_TYPE>(value * Amp + DCOffset);
            }
        }
    }

#endif//MIDILAR_PERIODIC_SAW_H
