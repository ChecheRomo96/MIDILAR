#ifndef MIDILAR_GENERATORS_WINDOWING_HAMMING_H
#define MIDILAR_GENERATORS_WINDOWING_HAMMING_H

#include <MIDILAR_BuildSettings.h>

#define __USE_MATH_DEFINES
#include <math.h>

namespace MIDILAR::DspCore::Generators::Windowing {

    template<typename T>
    void Hamming(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        for (size_t n = 0; n < BufferSize; ++n) {
            Buffer[n] = static_cast<T>(
                0.54f - 0.46f * cosf(
                    TWO_PI *
                    static_cast<float>(n) /
                    static_cast<float>(BufferSize - 1)
                )
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_HAMMING_H
