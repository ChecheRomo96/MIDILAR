#ifndef MIDILAR_DSP_FOUNDATION_H
#define MIDILAR_DSP_FOUNDATION_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/Generators/Generators.h>)

        #ifndef MIDILAR_GENERATORS
            #define MIDILAR_GENERATORS
        #endif

        #include "Generators.h"
    #endif
    
    #if __has_include(<dspFoundation/LUT/LUT.h>)

        #ifndef MIDILAR_LUT
            #define MIDILAR_LUT
        #endif

        #include "LUT.h"
    #endif
    
    #if __has_include(<dspFoundation/Interpolators/Interpolators.h>)

        #ifndef MIDILAR_INTERPOLATORS
            #define MIDILAR_INTERPOLATORS
        #endif

        #include "Interpolators.h"
    #endif
    
    #if __has_include(<dspFoundation/Streaming/Streaming.h>)

        #ifndef MIDILAR_STREAMING
            #define MIDILAR_STREAMING
        #endif

        #include "Streaming.h"
    #endif
    
#endif//MIDILAR_DSP_FOUNDATION_H