#ifndef MIDILAR_DSP_GENERATORS_PERIODIC_H
#define MIDILAR_DSP_GENERATORS_PERIODIC_H

    #include <MIDILAR_BuildSettings.h>

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_SINE
        #include "Sine.h"  
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_TRIANGLE
        #include "Triangle.h"
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_SAW
        #include "Saw.h"
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC_SQUARE
        #include "Square.h"
    #endif
    
#endif // MIDILAR_DSP_GENERATORS_PERIODIC_H
