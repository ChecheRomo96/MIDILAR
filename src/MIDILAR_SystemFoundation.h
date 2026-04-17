#ifndef MIDILAR_SYSTEM_FOUNDATION_TOP_H
#define MIDILAR_SYSTEM_FOUNDATION_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include(<SystemFoundation/SystemFoundation.h>)
        #ifndef MIDILAR_SYSTEM_FOUNDATION
            #define MIDILAR_SYSTEM_FOUNDATION   
        #endif

        #include <SystemFoundation/SystemFoundation.h>
    #endif

#endif//MIDILAR_SYSTEM_FOUNDATION_TOP_H