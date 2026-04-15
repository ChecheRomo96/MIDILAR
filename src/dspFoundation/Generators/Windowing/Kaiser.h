#ifndef MIDILAR_GENERATORS_WINDOWING_KAISER_H
#define MIDILAR_GENERATORS_WINDOWING_KAISER_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingMath.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Kaiser(T* Buffer, size_t BufferSize, float Beta = 0.5f) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        float Denominator = Internal::BesselI0(Beta);

        for (size_t n = 0; n < BufferSize; ++n) {
            float Ratio =
                (2.0f * static_cast<float>(n)) /
                static_cast<float>(BufferSize - 1) - 1.0f;

            float Argument = 1.0f - Ratio * Ratio;
            if (Argument < 0.0f) {
                Argument = 0.0f;
            }

            Buffer[n] = static_cast<T>(
                Internal::BesselI0(Beta * sqrtf(Argument)) / Denominator
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_KAISER_H
