#ifndef MIDILAR_GENERATORS_WINDOWING_GAUSSIAN_H
#define MIDILAR_GENERATORS_WINDOWING_GAUSSIAN_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Gaussian(T* Buffer, size_t BufferSize, float Alpha = 2.5f) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        float Half = static_cast<float>(BufferSize - 1) * 0.5f;

        for (size_t n = 0; n < BufferSize; ++n) {
            float X = (static_cast<float>(n) - Half) / (Alpha * Half);
            Buffer[n] = static_cast<T>(expf(-0.5f * X * X));
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_GAUSSIAN_H
