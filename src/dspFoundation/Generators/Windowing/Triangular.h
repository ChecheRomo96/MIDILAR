#ifndef MIDILAR_GENERATORS_WINDOWING_TRIANGULAR_H
#define MIDILAR_GENERATORS_WINDOWING_TRIANGULAR_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Triangular(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        float Half = static_cast<float>(BufferSize) * 0.5f;
        float Center = static_cast<float>(BufferSize - 1) * 0.5f;

        for (size_t n = 0; n < BufferSize; ++n) {
            Buffer[n] = static_cast<T>(
                1.0f - fabsf(
                    (static_cast<float>(n) - Center) / Half
                )
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_TRIANGULAR_H
