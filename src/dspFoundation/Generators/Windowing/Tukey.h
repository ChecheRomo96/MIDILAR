#ifndef MIDILAR_GENERATORS_WINDOWING_TUKEY_H
#define MIDILAR_GENERATORS_WINDOWING_TUKEY_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Tukey(T* Buffer, size_t BufferSize, float Alpha = 0.5f) {
        if (!Buffer || BufferSize == 0) return;

        if (Alpha <= 0.0f) {
            for (size_t n = 0; n < BufferSize; ++n) {
                Buffer[n] = static_cast<T>(1);
            }
            return;
        }

        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return;
        }

        if (Alpha >= 1.0f) {
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
            return;
        }

        float Edge = Alpha * static_cast<float>(BufferSize - 1) * 0.5f;

        for (size_t n = 0; n < BufferSize; ++n) {
            float Index = static_cast<float>(n);

            if (Index < Edge) {
                Buffer[n] = static_cast<T>(
                    0.5f * (
                        1.0f + cosf(
                            MIDILAR_PI * (
                                (2.0f * Index) /
                                (Alpha * static_cast<float>(BufferSize - 1))
                                - 1.0f
                            )
                        )
                    )
                );
            }
            else if (Index <= static_cast<float>(BufferSize - 1) - Edge) {
                Buffer[n] = static_cast<T>(1);
            }
            else {
                Buffer[n] = static_cast<T>(
                    0.5f * (
                        1.0f + cosf(
                            MIDILAR_PI * (
                                (2.0f * Index) /
                                (Alpha * static_cast<float>(BufferSize - 1))
                                - 2.0f / Alpha + 1.0f
                            )
                        )
                    )
                );
            }
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_TUKEY_H
