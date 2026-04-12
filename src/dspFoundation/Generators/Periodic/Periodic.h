#ifndef MIDILAR_DSP_GENERATORS_PERIODIC_H
#define MIDILAR_DSP_GENERATORS_PERIODIC_H

#include <System/BuildSettings.h>

#ifdef MIDILAR_GENERATORS_SINE
    #include "Sine/Sine.h"
#endif

#ifdef MIDILAR_GENERATORS_TRIANGLE
    #include "Triangle/Triangle.h"
#endif

#ifdef MIDILAR_GENERATORS_SAW
    #include "Saw/Saw.h"
#endif

#ifdef MIDILAR_GENERATORS_SQUARE
    #include "Square/Square.h"
#endif

#endif // MIDILAR_DSP_GENERATORS_PERIODIC_H
