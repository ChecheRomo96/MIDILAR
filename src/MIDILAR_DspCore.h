#ifndef MIDILAR_DSP_CORE_TOP_H
#define MIDILAR_DSP_CORE_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include("DspCore/DspCore.h")
        #ifndef MIDILAR_DSP_CORE
            #define MIDILAR_DSP_CORE   
        #endif

        #include "DspCore/DspCore.h"
    #endif
    

#endif//MIDILAR_DSP_CORE_TOP_H