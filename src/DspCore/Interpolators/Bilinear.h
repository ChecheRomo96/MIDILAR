#ifndef MIDILAR_INTERPOLATORS_TOP_H
#define MIDILAR_INTERPOLATORS_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include("Bilinear/Bilinear.h")
        #define MIDILAR_DSP_INTERPOLATORS_BILINEAR
        #include "Bilinear/Bilinear.h"
    #endif

#endif//MIDILAR_INTERPOLATORS_TOP_H