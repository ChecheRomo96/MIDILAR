#ifndef MIDILAR_PERIODIC_SINE_H
#define MIDILAR_PERIODIC_SINE_H

    #include <MIDILAR_BuildSettings.h>
    #include <stddef.h>

    #define _USE_MATH_DEFINES
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {
        template<typename T>
        void Sine(
            T* Buffer,
            size_t BufferSize,
            float Amp,
            float DcOffset,
            float PhaseOffset
        ) {
            if (!Buffer || BufferSize == 0) {
                return;
            }

            float twoPi = static_cast<float>(2.0 * M_PI);
            float phaseStep = twoPi / static_cast<float>(BufferSize);
            float phaseOffsetScaled = PhaseOffset * twoPi;

            for (size_t i = 0; i < BufferSize; ++i) {
                float phase = (static_cast<float>(i) * phaseStep) + phaseOffsetScaled;
                Buffer[i] = static_cast<T>(Amp * sinf(phase) + DcOffset);
            }
        }
    } // namespace MIDILAR::dspFoundation::Generators::Periodic

    #if defined(MIDILAR_DSP_LUT1D)

        #include <dspFoundation/LUT/LUT1D.h>
        
        namespace MIDILAR::dspFoundation::Generators::Periodic {
                template<typename T>
                bool SineLUT(
                    MIDILAR::dspFoundation::LUT::LUT1D<T>& lut,
                    float Amp = 1.0f,
                    float DcOffset = 0.0f,
                    float PhaseOffset = 0.0f
                ) {
                    if (!lut.GetBuffer() || lut.Size() == 0) {
                        return false;
                    }

                    Sine<T>(
                        lut.GetBuffer(),
                        lut.Size(),
                        Amp,
                        DcOffset,
                        PhaseOffset
                    );

                    return true;
                }
        } // namespace MIDILAR::dspFoundation::Generators::Periodic

    #endif // MIDILAR_DSP_LUT1D

#endif // MIDILAR_PERIODIC_SINE_H