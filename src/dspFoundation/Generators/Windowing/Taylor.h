#ifndef MIDILAR_GENERATORS_WINDOWING_TAYLOR_H
#define MIDILAR_GENERATORS_WINDOWING_TAYLOR_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing {

    template<typename T>
    void Taylor(T* Buffer, size_t BufferSize, float SidelobeLevel = 30.0f, size_t NBar = 4) {
        (void)SidelobeLevel;
        (void)NBar;

        if (!Buffer || BufferSize == 0) return;

        for (size_t n = 0; n < BufferSize; ++n) {
            Buffer[n] = static_cast<T>(1);
        }
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_TAYLOR_H
