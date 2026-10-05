#ifndef MIDILAR_DEVICES_TOP_LEVEL_H
#define MIDILAR_DEVICES_TOP_LEVEL_H

#include <MIDILAR_BuildSettings.h>

#if __has_include(<MIDILAR/Devices.h>)
    #ifndef MIDILAR_DEVICES
        #define MIDILAR_DEVICES
    #endif

    #include <MIDILAR/Devices.h>
#endif

#endif // MIDILAR_DEVICES_TOP_LEVEL_H
