#ifndef MIDILAR_LUT_PERIODIC_TOP_H
#define MIDILAR_LUT_PERIODIC_TOP_H
    
    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<dspFoundation/LUT/Periodic/LUTPeriodic.h>)
        #define MIDILAR_LUT_PERIODIC
        #include <dspFoundation/LUT/Periodic/LUTPeriodic.h>
    #endif

#endif //MIDILAR_LUT_PERIODIC_TOP_H