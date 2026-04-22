#ifndef MIDILAR_DSP_GENERATORS_NOISE_TOP_H
#define MIDILAR_DSP_GENERATORS_NOISE_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/Generators/Noise/Noise.h>)
        #define MIDILAR_GENERATORS_NOISE
        #include <dspFoundation/Generators/Noise/Noise.h>
    #endif
    
#endif//MIDILAR_DSP_GENERATORS_NOISE_TOP_H