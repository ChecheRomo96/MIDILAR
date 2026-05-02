#ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER_H
#define MIDILAR_SYSTEM_CALLBACK_HANDLER_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<SystemCore/CallbackHandler/CallbackHandler.h>)
        #define MIDILAR_SYSTEM_CALLBACK_HANDLER
        #include <SystemCore/CallbackHandler/CallbackHandler.h>
    #endif
    
#endif//MIDILAR_SYSTEM_CALLBACK_HANDLER_H