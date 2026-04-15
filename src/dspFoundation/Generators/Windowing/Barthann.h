#ifndef MIDILAR_GENERATORS_WINDOWING_BARTHANN_H
#define MIDILAR_GENERATORS_WINDOWING_BARTHANN_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Barthann(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        for (size_t n = 0; n < BufferSize; ++n) {
            float Ratio = static_cast<float>(n) / static_cast<float>(BufferSize - 1);
            float Centered = Ratio - 0.5f;

            Buffer[n] = static_cast<T>(
                0.62f
                - 0.48f * fabsf(Centered)
                - 0.38f * cosf(MIDILAR_TWO_PI * Ratio)
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_BARTHANN_H
