#ifndef MIDILAR_SYSTEM_CLOCK_H
#define MIDILAR_SYSTEM_CLOCK_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<SystemCore/Clock/Clock.h>)
        #define MIDILAR_SYSTEM_CLOCK
        #include <SystemCore/Clock/Clock.h>
    #endif

#endif//MIDILAR_SYSTEM_CLOCK_H