#ifndef MIDILAR_DSP_TRANSFORMS_H
#define MIDILAR_DSP_TRANSFORMS_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/Transforms/Transforms.h>)
        #define MIDILAR_DSP_TRANSFORMS
        #include <dspFoundation/Transforms/Transforms.h>
    #endif

#endif//MIDILAR_DSP_TRANSFORMS_H