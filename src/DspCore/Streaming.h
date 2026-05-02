#ifndef MIDILAR_DSP_STREAMING_H
#define MIDILAR_DSP_STREAMING_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<DspCore/Streaming/Streaming.h>)
        #define MIDILAR_DSP_STREAMING
        #include <DspCore/Streaming/Streaming.h>
    #endif

#endif//MIDILAR_DSP_STREAMING_H