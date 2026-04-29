#ifndef MIDILAR_GENERATORS_SHAPING_TANH_TOP_H
#define MIDILAR_GENERATORS_SHAPING_TANH_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include("Tanh/Tanh.h")
        #ifndef MIDILAR_DSP_GENERATORS_SHAPING_TANH
            #define MIDILAR_DSP_GENERATORS_SHAPING_TANH
        #endif

        #include "Tanh/Tanh.h"  
    #endif

#endif//MIDILAR_GENERATORS_SHAPING_TANH_TOP_H