#ifndef MIDILAR_CORE_TOP_LEVEL_H
#define MIDILAR_CORE_TOP_LEVEL_H

#include <MIDILAR_BuildSettings.h>

#if __has_include(<MIDILAR/Core.h>)
    #ifndef MIDILAR_CORE
        #define MIDILAR_CORE
    #endif

    #include <MIDILAR/Core.h>
#endif

#endif // MIDILAR_CORE_TOP_LEVEL_H
