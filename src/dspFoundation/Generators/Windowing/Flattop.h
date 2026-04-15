#ifndef MIDILAR_GENERATORS_WINDOWING_FLATTOP_H
#define MIDILAR_GENERATORS_WINDOWING_FLATTOP_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Flattop(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        const float A0 = 1.0f;
        const float A1 = 1.93f;
        const float A2 = 1.29f;
        const float A3 = 0.388f;
        const float A4 = 0.028f;

        for (size_t n = 0; n < BufferSize; ++n) {
            float Ratio = static_cast<float>(n) / static_cast<float>(BufferSize - 1);
            float Phase1 = MIDILAR_TWO_PI * Ratio;
            float Phase2 = 2.0f * Phase1;
            float Phase3 = 3.0f * Phase1;
            float Phase4 = 4.0f * Phase1;

            Buffer[n] = static_cast<T>(
                A0
                - A1 * cosf(Phase1)
                + A2 * cosf(Phase2)
                - A3 * cosf(Phase3)
                + A4 * cosf(Phase4)
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_FLATTOP_H
