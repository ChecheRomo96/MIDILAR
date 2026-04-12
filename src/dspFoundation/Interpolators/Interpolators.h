#ifndef MIDILAR_DSP_INTERPOLATORS_H
#define MIDILAR_DSP_INTERPOLATORS_H

#include <System/BuildSettings.h>

    #ifdef MIDILAR_INTERPOLATORS_LINEAR
        #include "Linear/Linear.h"
    #endif

    #ifdef MIDILAR_INTERPOLATORS_BILINEAR
        #include "Bilinear/Bilinear.h"
    #endif

    #ifdef MIDILAR_INTERPOLATORS_TRILINEAR
        #include "Trilinear/Trilinear.h"
    #endif

#endif // MIDILAR_DSP_INTERPOLATORS_H
