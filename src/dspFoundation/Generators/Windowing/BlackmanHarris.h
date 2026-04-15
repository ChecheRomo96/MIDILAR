#ifndef MIDILAR_GENERATORS_WINDOWING_BLACKMAN_HARRIS_H
#define MIDILAR_GENERATORS_WINDOWING_BLACKMAN_HARRIS_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void BlackmanHarris(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        const float A0 = 0.35875f;
        const float A1 = 0.48829f;
        const float A2 = 0.14128f;
        const float A3 = 0.01168f;

        for (size_t n = 0; n < BufferSize; ++n) {
            float Ratio = static_cast<float>(n) / static_cast<float>(BufferSize - 1);
            float Phase1 = MIDILAR_TWO_PI * Ratio;
            float Phase2 = 2.0f * Phase1;
            float Phase3 = 3.0f * Phase1;

            Buffer[n] = static_cast<T>(
                A0
                - A1 * cosf(Phase1)
                + A2 * cosf(Phase2)
                - A3 * cosf(Phase3)
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_BLACKMAN_HARRIS_H
