#ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER_H
#define MIDILAR_SYSTEM_CALLBACK_HANDLER_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<SystemFoundation/CallbackHandler/CallbackHandler.h>)
        #define MIDILAR_SYSTEM_CALLBACK_HANDLER
        #include <SystemFoundation/CallbackHandler/CallbackHandler.h>
    #endif
    
#endif//MIDILAR_SYSTEM_CALLBACK_HANDLER_H