#ifndef MIDILAR_GENERATORS_WINDOWING_INTERNAL_COMMON_H
#define MIDILAR_GENERATORS_WINDOWING_INTERNAL_COMMON_H

#include <MIDILAR_BuildSettings.h>
#include <stddef.h>
#include <math.h>

#ifndef MIDILAR_PI
    #define MIDILAR_PI 3.14159265359f
#endif

#ifndef MIDILAR_TWO_PI
    #define MIDILAR_TWO_PI 6.28318530718f
#endif

namespace MIDILAR::dspFoundation::Generators::Windowing::Internal {

    template<typename T>
    inline bool ValidateBuffer(T* Buffer, size_t BufferSize) {
        if (!Buffer || BufferSize == 0) {
            return false;
        }

        if (BufferSize == 1) {
            Buffer[0] = static_cast<T>(1);
            return false;
        }

        return true;
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_INTERNAL_COMMON_H
