#ifndef MIDILAR_SYSTEM_FOUNDATION_H
#define MIDILAR_SYSTEM_FOUNDATION_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include(<SystemFoundation/CallbackHandler/CallbackHandler.h>)

        #ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER
            define MIDILAR_SYSTEM_CALLBACK_HANDLER
        #endif
        #include <SystemFoundation/CallbackHandler/CallbackHandler.h>
    #endif

    #if __has_include(<SystemFoundation/Clock/Clock.h>)
        #ifndef MIDILAR_SYSTEM_CLOCK
            #define MIDILAR_SYSTEM_CLOCK
        #endif
        #include <SystemFoundation/Clock/Clock.h>
    #endif
    
#endif//MIDILAR_SYSTEM_FOUNDATION_H