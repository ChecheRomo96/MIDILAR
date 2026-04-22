#ifndef MIDILAR_INTERPOLATORS_TOP_H
#define MIDILAR_INTERPOLATORS_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include("Linear/Linear.h")
        #define MIDILAR_DSP_INTERPOLATORS_LINEAR
        #include "Linear/Linear.h"
    #endif

#endif//MIDILAR_INTERPOLATORS_TOP_H