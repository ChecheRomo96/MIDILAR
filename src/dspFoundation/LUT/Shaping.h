#ifndef MIDILAR_LUT_SHAPING_TOP_H
#define MIDILAR_LUT_SHAPING_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/LUT/Shaping/Shaping.h>)
        #define MIDILAR_LUT_SHAPING
        #include <dspFoundation/LUT/Shaping/Shaping.h>
    #endif

#endif //MIDILAR_LUT_SHAPINGTOP_H