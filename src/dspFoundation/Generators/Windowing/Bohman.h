#ifndef MIDILAR_GENERATORS_WINDOWING_BOHMAN_H
#define MIDILAR_GENERATORS_WINDOWING_BOHMAN_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Bohman(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        for (size_t n = 0; n < BufferSize; ++n) {
            float Ratio = (2.0f * static_cast<float>(n)) / static_cast<float>(BufferSize - 1) - 1.0f;
            float AbsRatio = fabsf(Ratio);

            Buffer[n] = static_cast<T>(
                (1.0f - AbsRatio) * cosf(MIDILAR_PI * AbsRatio)
                + (1.0f / MIDILAR_PI) * sinf(MIDILAR_PI * AbsRatio)
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_BOHMAN_H
