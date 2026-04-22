#ifndef MIDILAR_DSP_GENERATORS_ENVELOPES_TOP_H
#define MIDILAR_DSP_GENERATORS_ENVELOPES_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/Generators/Envelopes/Envelopes.h>)
        #define MIDILAR_GENERATORS_ENVELOPES
        #include <dspFoundation/Generators/Envelopes/Envelopes.h>
    #endif
    
#endif//MIDILAR_DSP_GENERATORS_ENVELOPES_TOP_H