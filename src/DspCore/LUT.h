#ifndef MIDILAR_DSP_LUT_H
#define MIDILAR_DSP_LUT_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<DspCore/LUT/LUT.h>)
        #define MIDILAR_DSP_LUT
        #include <DspCore/LUT/LUT.h>
    #endif

#endif//MIDILAR_DSP_LUT_H