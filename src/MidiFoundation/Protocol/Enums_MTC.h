#ifndef MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_MTC_H
#define MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_MTC_H

    #include <System/BuildSettings.h>
    #include <stdint.h>
    #include "Defines.h"

    #if __has_include(<string>)
        #include <string>
    #endif

    /**
     * @namespace MIDILAR::MidiFoundation::Protocol::MTC
     * @brief Namespace for MIDI Time Code (MTC) protocol-related definitions and utilities.
     */
    namespace MIDILAR::MidiFoundation::Protocol::MTC {

        /**
         * @ingroup MTC_QuarterFrame
         * @brief Represents the MIDI Time Code (MTC) Quarter Frame message.
         * 
         * This message is part of the System Common messages and is used 
         * to transmit timing information in a compact format. 
         * The Quarter Frame message divides a full frame into eight segments, 
         * allowing precise synchronization for devices that require frame-level accuracy.
         * 
         * @note The value of this constant corresponds to the MIDI specification
         * for the MTC Quarter Frame message (0xF1).
         */
        static constexpr uint8_t QuarterFrame = MIDI_MTC_QUARTER_FRAME;


        /**
         * @ingroup MTC_TimeComponents
         * @enum TimeComponent
         * @brief Represents the individual components of time in the MIDI Time Code (MTC) protocol.
         *
         * This enum provides named constants for the least significant bits (LSB) and most significant bits (MSB)
         * of frames, seconds, minutes, and hours in MTC messages. These components allow precise 
         * representation and manipulation of time data in the MTC format.
         */
        enum class TimeComponent : uint8_t {
            FramesLSB       = MIDI_MTC_FRAMES_LSB,    ///< Least significant bits of Frames.
            FramesMSB       = MIDI_MTC_FRAMES_MSB,    ///< Most significant bits of Frames.
            SecondsLSB      = MIDI_MTC_SECONDS_LSB,   ///< Least significant bits of Seconds.
            SecondsMSB      = MIDI_MTC_SECONDS_MSB,   ///< Most significant bits of Seconds.
            MinutesLSB      = MIDI_MTC_MINUTES_LSB,   ///< Least significant bits of Minutes.
            MinutesMSB      = MIDI_MTC_MINUTES_MSB,   ///< Most significant bits of Minutes.
            HoursLSB        = MIDI_MTC_HOURS_LSB,     ///< Least significant bits of Hours.
            HoursMSB        = MIDI_MTC_HOURS_MSB,     ///< Most significant bits of Hours, including FrameRate.
        };

        
        /**
         * @ingroup MTC_FrameRates
         * @enum FrameRate
         * @brief Represents the frame rates supported by MIDI Time Code.
         */
        enum class FrameRate : uint8_t {
            FPS24          = MIDI_MTC_FRAME_RATE_24FPS,            ///< 24 frames per second.
            FPS25          = MIDI_MTC_FRAME_RATE_25FPS,            ///< 25 frames per second.
            FPS30DropFrame = MIDI_MTC_FRAME_RATE_30FPS_DROP_FRAME, ///< 30 drop-frame per second.
            FPS30          = MIDI_MTC_FRAME_RATE_30FPS             ///< 30 non-drop-frame per second.
        };

        /**
         * @ingroup MIDI_MTC
         * @class SongPosition
         * @brief Manages and manipulates song position data for MIDI Time Code (MTC).
         */
        class SongPosition {

            public:
                /**
                 * @struct PositionData
                 * @brief Represents the data structure for a song's position in MTC.
                 *
                 * Stores individual components of MTC, such as hours, minutes, seconds, and frames,
                 * along with the frame rate.
                 */
                struct PositionData {
                    public:
                        uint8_t Hours = 0;    ///< Hours (0-23 or 0-31, depending on the use case).
                        uint8_t Minutes = 0;  ///< Minutes (0-59).
                        uint8_t Seconds = 0;  ///< Seconds (0-59).
                        uint8_t Frames = 0;   ///< Frames (0 to frame limit based on FrameRate).
                        uint8_t FrameRate = 0;///< Frame rate identifier (24, 25, 30, etc.).

                        /**
                         * @brief Retrieves specific time component data.
                         * @param TC The TimeComponent to retrieve (e.g., FramesLSB, MinutesMSB).
                         * @return The requested component value.
                         * @ingroup MTC_TimeComponents
                         * @ingroup MTC_FrameRates
                         */
                        uint8_t GetData(const TimeComponent& TC) const;

                        /**
                         * @brief Checks if the data in the structure is valid.
                         * @return True if valid, false otherwise.
                         *
                         * Validates that all components are within their expected ranges,
                         * including proper frame rates and drop-frame edge cases.
                         */
                        bool isDataValid() const;

                        /**
                         * @brief Increases the frame count and handles overflow.
                         *
                         * Increments the `Frames` field. If the frame count exceeds the frame limit,
                         * it resets and increments the higher time units (e.g., Seconds, Minutes).
                         */
                        void IncreaseFrame();

                    private:
                        /**
                         * @brief Retrieves the frame limit based on the current frame rate.
                         * @return The maximum frame value for the current frame rate.
                         */
                        uint8_t GetFrameLimit() const;

                        /**
                         * @brief Increments the seconds and handles overflow to minutes and hours.
                         */
                        void IncrementSeconds();
                };

            protected:
                PositionData _position; ///< Internal representation of the current position.

            public:
                /**
                 * @brief Default constructor for SongPosition.
                 */
                SongPosition() = default;

                /**
                 * @brief Sets the current position data.
                 * @param data The new position data to set.
                 * @throws std::invalid_argument if the provided position data is invalid.
                 */
                void SetPosition(const PositionData& data);

                /**
                 * @brief Retrieves the current position data.
                 * @return A constant reference to the current position data.
                 */
                const PositionData& GetPosition() const;

                /**
                 * @brief Increments the frame in the current position.
                 *
                 * This method increments the frame count and handles overflow into
                 * seconds, minutes, and hours as needed.
                 */
                void IncrementFrame();
        };

    } // namespace MIDILAR::MidiFoundation::Protocol::MTC

    /** 
     * @brief Alias for `MIDILAR::MidiFoundation::Protocol::MTC`.
     * 
     * All functionality is defined in the `MIDILAR::MidiFoundation::Protocol::MTC` namespace.
     */
    namespace MidiProtocol_MTC = MIDILAR::MidiFoundation::Protocol::MTC;
    

#endif // MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_MTC_H