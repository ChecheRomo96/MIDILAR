#ifndef MIDILAR_DSP_INTERPOLATORS_H
#define MIDILAR_DSP_INTERPOLATORS_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/Interpolators/Interpolators.h>)
        #define MIDILAR_DSP_INTERPOLATORS
        #include <dspFoundation/Interpolators/Interpolators.h>
    #endif

#endif//MIDILAR_DSP_INTERPOLATORS_H