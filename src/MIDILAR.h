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
       
    #include "MIDILAR_Core.h"

    #include "MIDILAR_SystemCore.h"
    #include "MIDILAR_DspCore.h"
    #include "MIDILAR_MidiCore.h"
    #include "MIDILAR_MidiDevices.h"
    
#endif // MIDILAR_H
