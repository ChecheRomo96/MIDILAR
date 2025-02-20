#ifndef MIDILAR_PERIODIC_GENERATORS_H
#define MIDILAR_PERIODIC_GENERATORS_H

    #include <System/BuildSettings.h>
    #include <math.h>

    #ifndef TWO_PI
        #define TWO_PI 6.28318530718f
    #endif

    namespace MIDILAR::dspFoundation::LUT::Periodic {

        template<typename OUTPUT_TYPE>
        void SineGenerator(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DcOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;
    
            float phaseStep = TWO_PI / static_cast<float>(BufferSize);
            
            float phaseOffsetScaled = PhaseOffset * TWO_PI;  // Precompute once

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = (i * phaseStep) + phaseOffsetScaled;
                Buffer[i] = static_cast<OUTPUT_TYPE>(Amp * sinf(phase) + DcOffset);
            }
        }

        template<typename OUTPUT_TYPE>
        void SawtoothGenerator(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DCOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;

            float phaseStep = TWO_PI / static_cast<float>(BufferSize);

            // Normalize phase offset to [0, TWO_PI]
            PhaseOffset = fmodf(PhaseOffset, TWO_PI);
            if (PhaseOffset < 0.0f) PhaseOffset += TWO_PI;

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = fmodf(i * phaseStep + PhaseOffset, TWO_PI);
                Buffer[i] = static_cast<OUTPUT_TYPE>(((phase / M_PI) - 1.0f) * Amp + DCOffset);
            }
        }

        template<typename OUTPUT_TYPE>
        void SquareGenerator(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DcOffset, float PhaseOffset) {
            if (!this->_buffer || BufferSize == 0) return;
    
            float phaseStep = TWO_PI / static_cast<float>(BufferSize);
            
            // Normalize phase offset to [0, TWO_PI]
            PhaseOffset = fmodf(PhaseOffset, TWO_PI);
            if (PhaseOffset < 0.0f) PhaseOffset += TWO_PI;
    
            for (size_t i = 0; i < BufferSize; i++) {
                float phase = fmodf(i * phaseStep + PhaseOffset, 1.0f);
    
                Buffer[i] = static_cast<OUTPUT_TYPE>(
                    (phase < 0.5f) ? Amp + DcOffset : -Amp + DcOffset
                );
            }
        }

        template<typename OUTPUT_TYPE>
        void TriangleGenerator(OUTPUT_TYPE* Buffer, size_t BufferSize, float Amp, float DCOffset, float PhaseOffset) {
            if (!Buffer || BufferSize == 0) return;

            float phaseStep = TWO_PI / static_cast<float>(BufferSize);

            // Normalize phase offset to [0, TWO_PI]
            PhaseOffset = fmodf(PhaseOffset, TWO_PI);
            if (PhaseOffset < 0.0f) PhaseOffset += TWO_PI;

            for (size_t i = 0; i < BufferSize; i++) {
                float phase = fmodf(i * phaseStep + PhaseOffset, TWO_PI);

                // Normalize phase to [0,1] range
                float normalizedPhase = phase / TWO_PI;

                // Triangle wave equation (mirroring around 0.5 for symmetry)
                float triangleValue = 2.0f * fabsf(2.0f * normalizedPhase - 1.0f) - 1.0f;

                Buffer[i] = static_cast<OUTPUT_TYPE>(triangleValue * Amp + DCOffset);
            }
        }

    }

#endif//MIDILAR_PERIODIC_GENERATORS_H
