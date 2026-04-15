#ifndef MIDILAR_GENERATORS_WINDOWING_RECTANGULAR_H
#define MIDILAR_GENERATORS_WINDOWING_RECTANGULAR_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Rectangular(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) return;

        for (size_t n = 0; n < BufferSize; ++n) {
            Buffer[n] = static_cast<T>(1);
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_RECTANGULAR_H
