#ifndef MIDILAR_SYSTEM_FOUNDATION_H
#define MIDILAR_SYSTEM_FOUNDATION_H

    #include <System/BuildSettings.h>
    
    #ifdef MIDILAR_CALLBACK_HANDLER
        #include <SystemFoundation/CallbackHandler/CallbackHandler.h>
    #endif

    #ifdef MIDILAR_SYSTEM_CLOCK
        #include <SystemFoundation/Clock/Clock.h>
    #endif


#endif//MIDILAR_SYSTEM_FOUNDATION_H