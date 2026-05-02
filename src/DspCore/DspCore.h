#ifndef MIDILAR_DSP_CORE_H
#define MIDILAR_DSP_CORE_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<DspCore/Generators/Generators.h>)

        #ifndef MIDILAR_DSP_GENERATORS
            #define MIDILAR_DSP_GENERATORS
        #endif

        #include "Generators.h"
    #endif
    
    #if __has_include(<DspCore/LUT/LUT.h>)

        #ifndef MIDILAR_DSP_LUT
            #define MIDILAR_DSP_LUT
        #endif

        #include "LUT.h"
    #endif
    
    #if __has_include("DspCore/Interpolators/Interpolators.h")

        #ifndef MIDILAR_DSP_INTERPOLATORS
            #define MIDILAR_DSP_INTERPOLATORS
        #endif

        #include "Interpolators.h"
    #endif
    
    #if __has_include("DspCore/Streaming/Streaming.h")

        #ifndef MIDILAR_DSP_STREAMING
            #define MIDILAR_DSP_STREAMING
        #endif

        #include "Streaming.h"
    #endif
    
#endif//MIDILAR_DSP_CORE_H