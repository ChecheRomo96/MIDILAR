#ifndef MIDILAR_GENERATORS_ATAN_TOP_H
#define MIDILAR_GENERATORS_ATAN_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include("Atan/Atan.h")
        #ifndef MIDILAR_DSP_GENERATORS_SHAPING_ATAN
            #define MIDILAR_DSP_GENERATORS_SHAPING_ATAN
        #endif

        #include "Atan/Atan.h"  
    #endif

#endif//MIDILAR_GENERATORS_ATAN_TOP_H