#ifndef MIDILAR_LUT1D_TOP_H
#define MIDILAR_LUT1D_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<DspCore/LUT/LUT1D/LUT1D.h>)
        #ifndef MIDILAR_DSP_LUT1D
            #define MIDILAR_DSP_LUT1D
        #endif
        #include <DspCore/LUT/LUT1D/LUT1D.h>
    #endif

#endif //MIDILAR_LUT1D_TOP_H