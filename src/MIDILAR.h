/**
 * @file MIDILAR.h
 * @brief Main include file for the MIDILAR library.
 * 
 * This header file serves as the central point for including all necessary headers
 * for using the MIDILAR library. It conditionally includes additional modules
 * based on the build settings.
 * 
 * @details
 * - Includes build settings defined in `MIDILAR_BuildSettings.h`.
 * - Optionally includes system tools if `MIDILAR_SYSTEM_TOOLS_ENABLED` is defined.
 * 
 * @author José Manuel Romo Peredo
 * @date 2024-06-01
 * @version 0.0.1
 */

#ifndef MIDILAR_H
#define MIDILAR_H

    /** 
     * @brief Include the build settings for MIDILAR.
     */
    #include "MIDILAR_BuildSettings.h"

    
    #if __has_include(<MIDILAR_SystemFoundation.h>)
        #ifndef MIDILAR_SYSTEM_FOUNDATION
            #define MIDILAR_SYSTEM_FOUNDATION
        #endif
        #include <MIDILAR_SystemFoundation.h>
    #endif
    
    #if __has_include(<MIDILAR_MidiFoundation.h>)
        #ifndef MIDILAR_MIDI_FOUNDATION
            #define MIDILAR_MIDI_FOUNDATION
        #endif
        #include <MIDILAR_MidiFoundation.h>
    #endif
    
    #if __has_include(<MIDILAR_dspFoundation.h>)
        #ifndef MIDILAR_DSP_FOUNDATION
            #define MIDILAR_DSP_FOUNDATION
        #endif
        #include <MIDILAR_dspFoundation.h>
    #endif

    #if __has_include(<MIDILAR_MidiDevices.h>)
        #ifndef MIDILAR_MIDI_DEVICES
            #define MIDILAR_MIDI_DEVICES
        #endif
        #include <MIDILAR_MidiDevices.h>
    #endif

#endif // MIDILAR_H
