/**
 * @file MIDILAR_MIDI_MTC_DEFINES.h
 * @brief Defines constants for MIDI Time Code (MTC) and related SysEx messages.
 * 
 * This file provides definitions for:
 * 
 * - Standard MIDI Time Code commands
 * - MIDI Time Code SysEx messages for Non-Real-Time and Real-Time operations
 */

#ifndef MIDILAR_MIDI_MTC_DEFINES_H
#define MIDILAR_MIDI_MTC_DEFINES_H


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MTC

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup MTC_QuarterFrame
         * @brief Defines the status byte for an MTC (MIDI Time Code) Quarter Frame message.
         *
         * The MTC Quarter Frame message is a two-byte MIDI message used to transmit time code information
         * in small increments. It follows the structure:
         * 
         * ```
         * [MIDI_MTC_QUARTER_FRAME] [Data Byte]
         * ```
         *
         * - **Status Byte (0xF1)**: Indicates an MTC Quarter Frame message.
         * - **Data Byte (0b0000NNND)**:
         *   - `NNN` (Bits 6-4): Time component identifier (0-7, representing different parts of the SMPTE time code).
         *   - `D` (Bits 3-0): The actual data value for the given time component.
         *
         * ### Usage Example
         * To construct a raw MTC Quarter Frame message:
         * ```cpp
         * uint8_t timeComponent = 2; // Example: Frame LSB
         * uint8_t data = 5;          // Example data value
         * uint8_t message[2] = { MIDI_MTC_QUARTER_FRAME, (timeComponent << 4) | (data & 0x0F) };
         * ```
         *
         * @note The full SMPTE time code is transmitted over eight successive Quarter Frame messages.
         */
            #define MIDI_MTC_QUARTER_FRAME 0xF1
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Subgroups

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MTC_TimeComponents
             */
            /**@{*/ // Start of MTC_TimeComponents subgroup

                #define MIDI_MTC_FRAMES_LSB 0x00  ///< Lower 4 bits of frame number.
                #define MIDI_MTC_FRAMES_MSB 0x10  ///< Upper 4 bits of frame number.
                #define MIDI_MTC_SECONDS_LSB 0x20 ///< Lower 4 bits of seconds.
                #define MIDI_MTC_SECONDS_MSB 0x30 ///< Upper 4 bits of seconds.
                #define MIDI_MTC_MINUTES_LSB 0x40 ///< Lower 4 bits of minutes.
                #define MIDI_MTC_MINUTES_MSB 0x50 ///< Upper 4 bits of minutes.
                #define MIDI_MTC_HOURS_LSB 0x60   ///< Lower 4 bits of hours.
                #define MIDI_MTC_HOURS_MSB 0x70   ///< Upper 4 bits of hours and frame rate.
            /**@}*/ // End of MTC_TimeComponents subgroup
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MTC_FrameRates
             */
            /**@{*/ // Start of MTC_FrameRates subgroup

                #define MIDI_MTC_FRAME_RATE_24FPS 0x00            ///< 24 frames per second.
                #define MIDI_MTC_FRAME_RATE_25FPS 0x02            ///< 25 frames per second.
                #define MIDI_MTC_FRAME_RATE_30FPS_DROP_FRAME 0x04 ///< 30 FPS (drop frame).
                #define MIDI_MTC_FRAME_RATE_30FPS 0x06            ///< 30 FPS (non-drop frame).
            /**@}*/ // End of MTC_FrameRates subgroup
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // MTC_SysEx

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MTC_NonRealTime
             */
            /**@{*/ // Start of NonRealTime subgroup

            /**@}*/ // End of NonRealTime subgroup
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Subgroups

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Special
                 * @brief Indicates a special event type for Non-Real-Time MTC (MIDI Time Code) cueing.
                 * 
                 * This event type is reserved for specific or undefined functions within the 
                 * Non-Real-Time MTC cueing system. It acts as a general placeholder for 
                 * "special" scenarios or vendor-specific extensions.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_SPECIAL            0x00
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PunchPoints
                 * @brief Represents a punch-in point for Non-Real-Time MTC cueing.
                 * 
                 * This is used to mark the start point where recording or playback begins 
                 * during a punch-in process.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_PUNCH_IN_POINTS    0x01
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PunchPoints
                 * @brief Represents a punch-out point for Non-Real-Time MTC cueing.
                 * 
                 * This is used to mark the endpoint where recording or playback stops 
                 * during a punch-out process.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_PUNCH_OUT_POINTS   0x02
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PunchPoints
                 * @brief Deletes an existing punch-in point in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_DELETE_PUNCH_IN    0x03
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PunchPoints
                 * @brief Deletes an existing punch-out point in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_DELETE_PUNCH_OUT   0x04
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Marks the start point of an event in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_EVENT_START        0x05
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Marks the stop point of an event in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_EVENT_STOP         0x06
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Marks an event start point with additional metadata for Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_EVENT_START_INFO   0x07
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Marks an event stop point with additional metadata for Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_EVENT_STOP_INFO    0x08
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Deletes an event start point in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_DELETE_EVENT_START 0x09
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Deletes an event stop point in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_DELETE_EVENT_STOP  0x0A
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Cues
                 * @brief Defines general cue points in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_CUE_POINTS         0x0B 
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Cues
                 * @brief Defines cue points with additional metadata in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_CUE_POINTS_INFO    0x0C
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Cues
                 * @brief Deletes a specific cue point in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_DELETE_CUE_POINT   0x0D
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup Events
                 * @brief Provides an event name along with additional metadata in Non-Real-Time MTC cueing.
                 */
                    #define MIDI_SYSEX_NRT_MTC_CUEING_EVENT_NAME_INFO    0x0E
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // MTC Sysex Real Time Messages

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup MTC_RealTime
                 * @brief Provides an event name along with additional metadata in Non-Real-Time MTC cueing.
                 * 
                 * This macro defines the identifier for Real-Time MIDI Time Code (MTC) messages, specifically 
                 * used in Non-Real-Time cueing. It facilitates synchronization by providing detailed event 
                 * names and metadata, ensuring accurate timing and control across MIDI devices.
                 * 
                 * ### Definition
                 * - **Macro**: `MIDI_SYSEX_REALTIME_MTC`
                 * - **Value**: `0x01` (Represents the Real-Time MTC Sysex Identifier)
                 * 
                 * ### Use Cases
                 * - Synchronizing devices using MIDI Time Code in real-time applications.
                 * - Sending metadata-rich events for cue-based automation.
                 * - Integrating precise event timing into DAW workflows or live performances.
                 */
                    #define MIDI_SYSEX_REALTIME_MTC 0x01
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Sub Groups

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Commands

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @ingroup MTC_RT_Commands
                     * @defgroup MTC_RT_FullFrame MTC Full Frame Message
                     * @brief Represents the MTC Full Frame message identifier (Sub ID 2: 0x01).
                     * 
                     * The MIDI Time Code (MTC) Full Frame message is a System Exclusive (SysEx) message 
                     * used for precise synchronization of MIDI devices. It provides the complete 
                     * SMPTE timecode information, including hours, minutes, seconds, and frames, 
                     * enabling accurate time alignment between MIDI devices, DAWs, and other 
                     * time-sensitive systems.
                     * 
                     * ## Message Format
                     * The Full Frame message follows the MIDI System Exclusive (SysEx) structure:
                     * 
                     * | Byte | Value                                  | Description |
                     * |------|----------------------------------------|-------------|
                     * | 0xF0 | MIDI_SYSEX_START                      | Start of SysEx message |
                     * | 0x7F | Universal Real-Time SysEx ID          | Universal real-time message |
                     * | `ID` | Device ID (usually 0x7F for all devices) | Target device ID |
                     * | 0x01 | MIDI_SYSEX_REALTIME_MTC               | MTC real-time message ID |
                     * | 0x01 | MIDI_SYSEX_RT_MTC_FULL_FRAME          | Full Frame message identifier |
                     * | `hh` | Hours (including frame rate bits)     | 5-bit hours + 2-bit frame rate |
                     * | `mm` | Minutes (0-59)                        | Minutes component |
                     * | `ss` | Seconds (0-59)                        | Seconds component |
                     * | `ff` | Frames (0-29, based on frame rate)    | Frames component |
                     * | 0xF7 | MIDI_SYSEX_END                        | End of SysEx message |
                     * 
                     * ## Encoding Hours and Frame Rate
                     * The Hours byte (`hh`) includes the frame rate information in its upper 2 bits:
                     * 
                     * - **Frame Rate Encoding (upper 2 bits of `hh`)**:
                     *   - `00`: 24 frames per second (FPS)
                     *   - `01`: 25 FPS
                     *   - `10`: 30 FPS (drop-frame)
                     *   - `11`: 30 FPS (non-drop-frame)
                     * 
                     * - **Hours Encoding (lower 5 bits of `hh`)**:
                     *   - Range: `0x00` (0 hours) to `0x17` (23 hours)
                     * 
                     * ## Example: Constructing a Full Frame Message in Raw MIDI
                     * The following example builds an MTC Full Frame message for 01:23:45:12 at 30 FPS:
                     * 
                     * ```cpp
                     * uint8_t sysexMessage[] = {
                     *     0xF0,  // SysEx start
                     *     0x7F,  // Universal real-time SysEx ID (all devices)
                     *     0x7F,  // Target device (broadcast)
                     *     0x01,  // MIDI_SYSEX_REALTIME_MTC
                     *     0x01,  // MIDI_SYSEX_RT_MTC_FULL_FRAME
                     *     (0b011 << 5) | 0x01, // 30 FPS (non-drop-frame) + Hours (1)
                     *     0x23,  // Minutes (0x23 = 35 decimal)
                     *     0x3B,  // Seconds (0x3B = 59 decimal)
                     *     0x12,  // Frames (0x12 = 18 decimal)
                     *     0xF7   // SysEx end
                     * };
                     * ```
                     * 
                     * ## Use Cases
                     * - Synchronizing MIDI-compatible video or audio playback devices.
                     * - Aligning hardware sequencers, DAWs, or external devices in real-time workflows.
                     * - Providing a complete time reference during live performances or automated playback.
                     * 
                     * ## Notes
                     * - The Full Frame message is part of the **real-time MIDI specification**, meaning it 
                     *   should be processed immediately by receiving devices.
                     * - This message provides an absolute SMPTE timecode, unlike MTC Quarter Frame messages, 
                     *   which transmit timecode incrementally.
                     * - The `SysexChannel` parameter (0x7F) is typically used for broadcast but can be 
                     *   set to a specific device ID for targeted communication.
                     * 
                     * @see MIDI_SYSEX_REALTIME_MTC
                     * @see MTC_QuarterFrame
                     * @see MTC_FrameRates
                     * 
                     * @{
                     */
                        #define MIDI_SYSEX_RT_MTC_FULL_FRAME 0x01
                    /**@}*/
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @ingroup MTC_RT_Commands
                     * @brief Represents the MTC User Bits message identifier (Sub ID 2: 0x02).
                     * 
                     * The User Bits message allows transmission of user-defined metadata alongside MIDI Time Code (MTC). 
                     * These bits can include additional information, such as scene markers, project identifiers, or 
                     * other custom data relevant to the synchronized devices.
                     * 
                     * ### Use Cases
                     * - Embedding custom metadata in real-time MIDI streams for advanced synchronization setups.
                     * - Communicating additional identifiers or markers to DAWs or other hardware.
                     * - Enhancing live performance setups with custom time-based data.
                     */
                        #define MIDI_SYSEX_RT_MTC_USER_BITS 0x02
                    //
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Cueing

                /**
                 * @addtogroup MTC_RT_Cueing
                 */
                /**@{*/
                    #define MIDI_SYSEX_RT_MTC_CUEING                          0x05  ///< Sub ID 1: Cueing
                    #define MIDI_SYSEX_RT_MTC_CUEING_SPECIAL                  0x00  ///< Sub ID 2: Special Events
                    #define MIDI_SYSEX_RT_MTC_CUEING_PUNCH_IN_POINTS          0x01  ///< Punch In Points
                    #define MIDI_SYSEX_RT_MTC_CUEING_PUNCH_OUT_POINTS         0x02  ///< Punch Out Points
                    #define MIDI_SYSEX_RT_MTC_CUEING_EVENT_START_POINTS       0x05  ///< Event Start Points
                    #define MIDI_SYSEX_RT_MTC_CUEING_EVENT_STOP_POINTS        0x06  ///< Event Stop Points
                    #define MIDI_SYSEX_RT_MTC_CUEING_EVENT_START_POINTS_INFO  0x07  ///< Event Start Points with Additional Info
                    #define MIDI_SYSEX_RT_MTC_CUEING_EVENT_STOP_POINTS_INFO   0x08  ///< Event Stop Points with Additional Info
                    #define MIDI_SYSEX_RT_MTC_CUEING_CUE_POINTS               0x0B  ///< Cue Points
                    #define MIDI_SYSEX_RT_MTC_CUEING_CUE_POINTS_INFO          0x0C  ///< Cue Points with Additional Info
                    #define MIDI_SYSEX_RT_MTC_CUEING_EVENT_NAME               0x0E  ///< Event Name
                /**@}*/ // End of Cueing
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //    
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
#endif // MIDILAR_MIDI_MTC_DEFINES_H
