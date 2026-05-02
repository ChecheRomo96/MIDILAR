#ifndef MIDILAR_SYSTEM_CORE_TOP_H
#define MIDILAR_SYSTEM_CORE_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include(<SystemCore/SystemCore.h>)
        #ifndef MIDILAR_SYSTEM_CORE
            #define MIDILAR_SYSTEM_CORE   
        #endif

        #include <SystemCore/SystemCore.h>
    #endif

#endif//MIDILAR_SYSTEM_CORE_TOP_H