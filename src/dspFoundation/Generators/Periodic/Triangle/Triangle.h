#ifndef MIDILAR_PERIODIC_TRIANGLE_H
#define MIDILAR_PERIODIC_TRIANGLE_H

    #include <MIDILAR_BuildSettings.h>
    #include <stddef.h>
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {

        template<typename T>
        void Triangle(
            T* Buffer,
            size_t BufferSize,
            float Amp,
            float DCOffset,
            float PhaseOffset
        ) {
            if (!Buffer || BufferSize == 0) {
                return;
            }

            float phaseStep = 1.0f / static_cast<float>(BufferSize);

            // +0.75 starts at midpoint with rising slope.
            float phase = fmodf(PhaseOffset + 0.75f, 1.0f);
            if (phase < 0.0f) {
                phase += 1.0f;
            }

            for (size_t i = 0; i < BufferSize; ++i) {
                float triangleValue = 2.0f * fabsf(2.0f * phase - 1.0f) - 1.0f;

                Buffer[i] = static_cast<T>(triangleValue * Amp + DCOffset);

                phase += phaseStep;

                if (phase >= 1.0f) {
                    phase -= 1.0f;
                }
            }
        }

    } // namespace MIDILAR::dspFoundation::Generators::Periodic

    #if defined(MIDILAR_DSP_LUT1D)

        #include <dspFoundation/LUT/LUT1D.h>

        namespace MIDILAR::dspFoundation::Generators::Periodic {

            template<typename T>
            bool TriangleLUT(
                MIDILAR::dspFoundation::LUT::LUT1D<T>& lut,
                float Amp = 1.0f,
                float DCOffset = 0.0f,
                float PhaseOffset = 0.0f
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                Triangle<T>(
                    lut.GetBuffer(),
                    lut.Size(),
                    Amp,
                    DCOffset,
                    PhaseOffset
                );

                return true;
            }

        } // namespace MIDILAR::dspFoundation::Generators::Periodic

    #endif // MIDILAR_DSP_LUT1D

#endif // MIDILAR_PERIODIC_TRIANGLE_H