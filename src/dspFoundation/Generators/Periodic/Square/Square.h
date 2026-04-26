#ifndef MIDILAR_PERIODIC_SQUARE_H
#define MIDILAR_PERIODIC_SQUARE_H

    #include <MIDILAR_BuildSettings.h>
    #include <stddef.h>
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Periodic {

        template<typename T>
        void Square(
            T* Buffer,
            size_t BufferSize,
            float Amp,
            float DCOffset,
            float PhaseOffset
        ) {
            if (!Buffer || BufferSize == 0) {
                return;
            }
            float twoPi = 2.0 * M_PI;
            float phaseStep = twoPi / static_cast<float>(BufferSize);

            float phase = fmodf(PhaseOffset * twoPi, twoPi);
            if (phase < 0.0f) {
                phase += twoPi;
            }

            for (size_t i = 0; i < BufferSize; ++i) {
                Buffer[i] = static_cast<T>(
                    (phase < M_PI) ? (Amp + DCOffset) : (-Amp + DCOffset)
                );

                phase += phaseStep;

                if (phase >= twoPi) {
                    phase -= twoPi;
                }
            }
        }
    } // namespace MIDILAR::dspFoundation::Generators::Periodic

    #if defined(MIDILAR_DSP_LUT1D)

        #include <dspFoundation/LUT/LUT1D.h>

        namespace MIDILAR::dspFoundation::Generators::Periodic {

            template<typename T>
            bool SquareLUT(
                MIDILAR::dspFoundation::LUT::LUT1D<T>& lut,
                float Amp = 1.0f,
                float DCOffset = 0.0f,
                float PhaseOffset = 0.0f
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                Square<T>(
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

#endif // MIDILAR_PERIODIC_SQUARE_H