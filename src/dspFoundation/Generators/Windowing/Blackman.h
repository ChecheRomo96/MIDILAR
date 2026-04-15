#ifndef MIDILAR_GENERATORS_WINDOWING_BLACKMAN_H
#define MIDILAR_GENERATORS_WINDOWING_BLACKMAN_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Blackman(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        const float A0 = 0.42f;
        const float A1 = 0.5f;
        const float A2 = 0.08f;

        for (size_t n = 0; n < BufferSize; ++n) {
            float Ratio = static_cast<float>(n) / static_cast<float>(BufferSize - 1);

            Buffer[n] = static_cast<T>(
                A0
                - A1 * cosf(MIDILAR_TWO_PI * Ratio)
                + A2 * cosf(2.0f * MIDILAR_TWO_PI * Ratio)
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_BLACKMAN_H
