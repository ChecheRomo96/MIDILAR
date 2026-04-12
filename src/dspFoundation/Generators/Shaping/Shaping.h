#ifndef MIDILAR_GENERATORS_SHAPING_H
#define MIDILAR_GENERATORS_SHAPING_H

#include <System/BuildSettings.h>

#ifdef MIDILAR_GENERATORS_GAMMA
    #include "Gamma/Gamma.h"
#endif

#ifdef MIDILAR_GENERATORS_ENHANCED_GAMMA
    #include "EnhancedGamma/EnhancedGamma.h"
#endif

#endif // MIDILAR_GENERATORS_SHAPING_H
