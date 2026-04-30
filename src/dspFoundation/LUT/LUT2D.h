#ifndef MIDILAR_LUT2D_TOP_H
#define MIDILAR_LUT2D_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/LUT/LUT2D/LUT2D.h>)
        #ifndef MIDILAR_LUT2D
            #define MIDILAR_LUT2D
        #endif
        #include <dspFoundation/LUT/LUT2D/LUT2D.h>
    #endif

#endif //MIDILAR_LUT2D_TOP_H