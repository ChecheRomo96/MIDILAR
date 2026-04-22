#ifndef MIDILAR_GENERATORS_WINDOWING_HANN_H
#define MIDILAR_GENERATORS_WINDOWING_HANN_H

#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Hann(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;
        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        for (size_t n = 0; n < BufferSize; ++n) {
            Buffer[n] = static_cast<T>(
                0.5f * (
                    1.0f - cosf(
                        MIDILAR_TWO_PI *
                        static_cast<float>(n) /
                        static_cast<float>(BufferSize - 1)
                    )
                )
            );
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_HANN_H
