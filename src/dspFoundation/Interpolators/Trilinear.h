#ifndef MIDILAR_INTERPOLATORS_TOP_H
#define MIDILAR_INTERPOLATORS_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include("Trilinear/Trilinear.h")
        #define MIDILAR_DSP_INTERPOLATORS_TRILINEAR
        #include "Trilinear/Trilinear.h"
    #endif

#endif//MIDILAR_INTERPOLATORS_TOP_H