#ifndef MIDILAR_GENERATORS_WINDOWING_PARZEN_H
#define MIDILAR_GENERATORS_WINDOWING_PARZEN_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Parzen(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        float Half = static_cast<float>(BufferSize - 1) * 0.5f;

        for (size_t n = 0; n < BufferSize; ++n) {
            float X = fabsf((static_cast<float>(n) - Half) / Half);
            float Value = 0.0f;

            if (X <= 0.5f) {
                Value = 1.0f - 6.0f * X * X + 6.0f * X * X * X;
            }
            else {
                float OneMinusX = 1.0f - X;
                Value = 2.0f * OneMinusX * OneMinusX * OneMinusX;
            }

            Buffer[n] = static_cast<T>(Value);
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_PARZEN_H
