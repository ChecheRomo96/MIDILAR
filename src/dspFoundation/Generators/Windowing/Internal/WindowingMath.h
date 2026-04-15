#ifndef MIDILAR_GENERATORS_WINDOWING_INTERNAL_MATH_H
#define MIDILAR_GENERATORS_WINDOWING_INTERNAL_MATH_H

#include <dspFoundation/Generators/Windowing/Internal/WindowingCommon.h>

namespace MIDILAR::dspFoundation::Generators::Windowing::Internal {

    inline float BesselI0(float X) {
        float Sum = 1.0f;
        float Term = 1.0f;
        float HalfX = X * 0.5f;

        for (size_t K = 1; K < 32; ++K) {
            Term *= HalfX / static_cast<float>(K);
            float Add = Term * Term;
            Sum += Add;

            if (Add < 1e-6f) {
                break;
            }
        }

        return Sum;
    }

}

#endif // MIDILAR_GENERATORS_WINDOWING_INTERNAL_MATH_H
