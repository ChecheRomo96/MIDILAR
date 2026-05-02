#ifndef MIDILAR_SYSTEM_CORE_H
#define MIDILAR_SYSTEM_CORE_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include(<SystemCore/CallbackHandler/CallbackHandler.h>)

        #ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER
            #define MIDILAR_SYSTEM_CALLBACK_HANDLER
        #endif
        #include <SystemCore/CallbackHandler/CallbackHandler.h>
    #endif

    #if __has_include(<SystemCore/Clock/Clock.h>)
        #ifndef MIDILAR_SYSTEM_CLOCK
            #define MIDILAR_SYSTEM_CLOCK
        #endif
        #include <SystemCore/Clock/Clock.h>
    #endif

    #if __has_include(<SystemCore/RingBuffer/RingBuffer.h>)
        #ifndef MIDILAR_SYSTEM_RING_BUFFER
            #define MIDILAR_SYSTEM_RING_BUFFER
        #endif
        #include <SystemCore/RingBuffer/RingBuffer.h>
    #endif
    
#endif//MIDILAR_SYSTEM_CORE_H