#ifndef MIDILAR_LUT1D_TOP_H
#define MIDILAR_LUT1D_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/LUT/LUT1D/LUT1D.h>)
        #define MIDILAR_LUT1D
        #include <dspFoundation/LUT/LUT1D/LUT1D.h>
    #endif

#endif //MIDILAR_LUT1D_TOP_H