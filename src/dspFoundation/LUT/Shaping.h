#ifndef MIDILAR_LUT_SHAPING_TOP_H
#define MIDILAR_LUT_SHAPING_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/LUT/Shaping/LUTShaping.h>)
        #define MIDILAR_LUT_SHAPING
        #include <dspFoundation/LUT/Shaping/LUTShaping.h>
    #endif

#endif //MIDILAR_LUT_SHAPINGTOP_H