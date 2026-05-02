#ifndef MIDILAR_LUT3D_TOP_H
#define MIDILAR_LUT3D_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<DspCore/LUT/LUT3D/LUT3D.h>)
        #define MIDILAR_LUT3D
        #include <DspCore/LUT/LUT3D/LUT3D.h>
    #endif

#endif //MIDILAR_LUT3D_TOP_H