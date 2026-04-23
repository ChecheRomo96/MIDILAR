#ifndef MIDILAR_GENERATORS_PERIODIC_SINE_TOP_H
#define MIDILAR_GENERATORS_PERIODIC_SINE_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include("Sine/Sine.h")
        #ifndef MIDILAR_DSP_GENERATORS_PERIODIC_SINE
            #define MIDILAR_DSP_GENERATORS_PERIODIC_SINE
        #endif

        #include "Sine/Sine.h"  
    #endif

#endif//MIDILAR_GENERATORS_PERIODIC_SINE_TOP_H