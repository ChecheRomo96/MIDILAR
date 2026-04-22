#ifndef MIDILAR_DSP_GENERATORS_H
#define MIDILAR_DSP_GENERATORS_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/Generators/Generators.h>)
        #define MIDILAR_DSP_GENERATORS
        #include <dspFoundation/Generators/Generators.h>
    #endif

#endif//MIDILAR_DSP_GENERATORS_H