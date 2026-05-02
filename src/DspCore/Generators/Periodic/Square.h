#ifndef MIDILAR_GENERATORS_PERIODIC_SQUARE_TOP_H
#define MIDILAR_GENERATORS_PERIODIC_SQUARE_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include("Square/Square.h")
        #ifndef MIDILAR_DSP_GENERATORS_PERIODIC_SQUARE
            #define MIDILAR_DSP_GENERATORS_PERIODIC_SQUARE
        #endif

        #include "Square/Square.h"  
    #endif

#endif//MIDILAR_GENERATORS_PERIODIC_SQUARE_TOP_H