#ifndef MIDILAR_DSP_FOUNDATION_TOP_H
#define MIDILAR_DSP_FOUNDATION_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include("dspFoundation/dspFoundation.h")
        #ifndef MIDILAR_DSP_FOUNDATION
            #define MIDILAR_DSP_FOUNDATION   
        #endif

        #include "dspFoundation/dspFoundation.h"
    #endif
    

#endif//MIDILAR_DSP_FOUNDATION_TOP_H