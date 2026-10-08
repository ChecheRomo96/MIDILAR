#ifndef MIDILAR_BUILD_SETTINGS_H
#define MIDILAR_BUILD_SETTINGS_H

// The libraries this one depends on. Including them from their src root
// lets the Arduino builder find them from any header of this library.
#include <Foundation_BuildSettings.h>
#include <MCC_BuildSettings.h>

#ifndef MIDILAR_VERSION
    #define MIDILAR_VERSION "0.3.0"
#endif

#ifndef MIDILAR_CPLUSPLUS
    #if defined(_MSVC_LANG)
        #define MIDILAR_CPLUSPLUS _MSVC_LANG
    #elif defined(__cplusplus)
        #define MIDILAR_CPLUSPLUS __cplusplus
    #else
        #define MIDILAR_CPLUSPLUS 0L
    #endif
#endif

#if !defined(DOXYGEN) && (MIDILAR_CPLUSPLUS < 201703L)
    #error "MIDILAR requires C++17 or newer"
#endif

#endif // MIDILAR_BUILD_SETTINGS_H
