#ifndef MIDILAR_LUT3D_TOP_H
#define MIDILAR_LUT3D_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<DspCore/LUT/LUT3D/LUT3D.h>)
        #ifndef MIDILAR_DSP_LUT3D
            #define MIDILAR_DSP_LUT3D
        #endif
        #include <DspCore/LUT/LUT3D/LUT3D.h>
    #endif

#endif //MIDILAR_LUT3D_TOP_H