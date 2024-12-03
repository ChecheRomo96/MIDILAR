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
 * @author [Your Name]
 * @date [Date]
 * @version 1.0.0
 */

#ifndef MIDILAR_H
#define MIDILAR_H

    /** 
     * @brief Include the build settings for MIDILAR.
     */
    #include <MIDILAR_BuildSettings.h>

    #ifdef MIDILAR_SYSTEM_TOOLS_ENABLED
        /**
         * @brief Include system tools if enabled in the build settings.
         * 
         * System tools provide additional utilities for the library.
         */
        #include <SystemTools/MIDILAR_SystemTools.h>
    #endif

#endif // MIDILAR_H
