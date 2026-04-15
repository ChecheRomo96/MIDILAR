#ifndef MIDILAR_PERIODIC_SQUARE_H
#define MIDILAR_PERIODIC_SQUARE_H

    #include <System/BuildSettings.h>
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {

        template<typename OUTPUT_TYPE>
        void Square(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DCOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;

            float twoPi = static_cast<float>(2 * M_PI);
            float phaseStep = twoPi / static_cast<float>(BufferSize);

            // Convert fractional cycle offset to radians
            float offsetRad = fmodf(PhaseOffset * twoPi, twoPi);
            if (offsetRad < 0.0f) offsetRad += twoPi;

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = fmodf(i * phaseStep + offsetRad, twoPi);

                // Start HIGH, switch to LOW at π (halfway)
                Buffer[i] = static_cast<OUTPUT_TYPE>(
                    (phase < M_PI) ? Amp + DCOffset : -Amp + DCOffset
                );
            }
        }
    }

#endif//MIDILAR_PERIODIC_SQUARE_H
