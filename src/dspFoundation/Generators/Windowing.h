#ifndef MIDILAR_GENERATORS_WINDOWING_H
#define MIDILAR_GENERATORS_WINDOWING_H

#include <System/BuildSettings.h>
#include <math.h>

#ifndef PI
    #define PI 3.14159265359f
#endif

#ifndef TWO_PI
    #define TWO_PI 6.28318530718f
#endif

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Hann(T* buffer, size_t size) {
        if (!buffer || size == 0) return;

        for (size_t n = 0; n < size; ++n) {
            buffer[n] = static_cast<T>(
                0.5f * (1.0f - cosf(TWO_PI * static_cast<float>(n) / static_cast<float>(size - 1)))
            );
        }
    }

    template<typename T>
    void Hamming(T* buffer, size_t size) {
        if (!buffer || size == 0) return;

        for (size_t n = 0; n < size; ++n) {
            buffer[n] = static_cast<T>(
                0.54f - 0.46f * cosf(TWO_PI * static_cast<float>(n) / static_cast<float>(size - 1))
            );
        }
    }

    template<typename T>
    void Blackman(T* buffer, size_t size) {
        if (!buffer || size == 0) return;

        for (size_t n = 0; n < size; ++n) {
            float a0 = 0.42f;
            float a1 = 0.5f;
            float a2 = 0.08f;
            float ratio = static_cast<float>(n) / static_cast<float>(size - 1);
            buffer[n] = static_cast<T>(
                a0 - a1 * cosf(TWO_PI * ratio) + a2 * cosf(2.0f * TWO_PI * ratio)
            );
        }
    }

    template<typename T>
    void Rectangular(T* buffer, size_t size) {
        if (!buffer || size == 0) return;

        for (size_t n = 0; n < size; ++n) {
            buffer[n] = static_cast<T>(1.0f);
        }
    }

} // namespace MIDILAR::Generators::Windows

#endif // MIDILAR_GENERATORS_WINDOWING_H
