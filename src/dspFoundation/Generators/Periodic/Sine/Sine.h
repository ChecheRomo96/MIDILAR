#ifndef MIDILAR_PERIODIC_SINE_H
#define MIDILAR_PERIODIC_SINE_H

    #include <System/BuildSettings.h>

    #define _USE_MATH_DEFINES
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {
        template<typename OUTPUT_TYPE>
        void Sine(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DcOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;
    
            float twoPi = static_cast<float>(2.0 * M_PI);
            float phaseStep = twoPi / static_cast<float>(BufferSize);
            
            float phaseOffsetScaled = PhaseOffset * twoPi;  // Precompute once

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = (i * phaseStep) + phaseOffsetScaled;
                Buffer[i] = static_cast<OUTPUT_TYPE>(Amp * sinf(phase) + DcOffset);
            }
        }
    }

#endif//MIDILAR_PERIODIC_SINE_H
