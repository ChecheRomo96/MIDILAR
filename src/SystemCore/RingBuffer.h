#ifndef MIDILAR_SYSTEM_RING_BUFFER_TOP_H
#define MIDILAR_SYSTEM_RING_BUFFER_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<SystemCore/RingBuffer/RingBuffer.h>)
        #define MIDILAR_SYSTEM_RING_BUFFER
        #include <SystemCore/RingBuffer/RingBuffer.h>
    #endif

#endif//MIDILAR_SYSTEM_RING_BUFFER_TOP_H