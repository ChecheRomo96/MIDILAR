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
       
    #ifdef MIDILAR_SYSTEM_FOUNDATION
        #include "MIDILAR_SystemFoundation.h"
    #endif

    #ifdef MIDILAR_DSP_FOUNDATION
        #include "MIDILAR_dspFoundation.h"
    #endif

    #ifdef MIDILAR_MIDI_FOUNDATION
        #include "MIDILAR_MidiFoundation.h"
    #endif

    #ifdef MIDILAR_MIDI_DEVICES
        #include "MIDILAR_MidiDevices.h"
    #endif
    

#endif // MIDILAR_H
