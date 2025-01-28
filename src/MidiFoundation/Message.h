#ifndef MIDILAR_MIDI_MESSAGE_H
#define MIDILAR_MIDI_MESSAGE_H

/**
 * @file MIDILAR_MidiMessage.h
 * @brief Provides the `Message` class for creating and managing MIDI messages.
 */

    #include <MIDILAR_BuildSettings.h>
    #include <stdint.h>

    #if __has_include(<vector>)
        #include <vector>
    #endif

    #include "Protocol/Defines.h"
    #include "Protocol/Enums.h"

    namespace MIDILAR::MidiFoundation{

        /**
         * @class Message{}
         */
        class Message {

        protected:
        #if __has_include(<vector>)
            std::vector<uint8_t> _Data;
        #else
            uint8_t* _Data;
            size_t _BufferSize;
            size_t _MessageSize;
        #endif


        public:

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name Constructors and Assignment Operators
             */
            ///@{
                Message() noexcept;                         ///< Default constructor.
                Message(const Message& MessageBuffer);      ///< Copy constructor.
                Message(Message&& MessageBuffer) noexcept;  ///< Move constructor.
                Message(uint8_t* Buffer, uint8_t size);        ///< Constructor from a raw character buffer.
                
                Message& operator=(const Message& Source);
                Message& operator=(Message&& Source) noexcept;

                #if __has_include(<vector>)
                    Message(const std::vector<uint8_t>& MessageBuffer);
                    Message(std::vector<uint8_t>&& MessageBuffer) noexcept;
                    Message& operator=(const std::vector<uint8_t>& MessageBuffer);
                    Message& operator=(std::vector<uint8_t>&& MessageBuffer) noexcept;
                #endif        
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name Data Access
             */
            ///@{
      
                const uint8_t* Buffer() const; ///< Returns a pointer to the first data element stored in the message
                const uint8_t Data(size_t index); ///< Returns the data stored at the specified index
                const size_t size() const;  ///< Returns the size of the message stored in the object.
                
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name Vector API
             * @brief Set of functions to access the data when std::vector is available.
             * This set of functions does not replace the Data Access group of functions but adds on to enhance the member data accessibility.
             */
            ///@{
                #if __has_include(<vector>)
                    const std::vector<uint8_t>& Vector() const; ///< Gets the internal data buffer.
                    std::vector<uint8_t>::iterator begin() noexcept; ///< Gets an iterator to the beginning of the buffer.
                    std::vector<uint8_t>::const_iterator begin() const noexcept; ///< Gets a constant iterator to the beginning of the buffer.
                    std::vector<uint8_t>::iterator end() noexcept; ///< Gets an iterator to the end of the buffer.
                    std::vector<uint8_t>::const_iterator end() const noexcept; ///< Gets a constant iterator to the end of the buffer.
                #endif            
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Channel Voice Messages

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup NoteMessages
                 * @brief Constructs a Note Off message.
                 *
                 * Sends a MIDI Note Off message to stop a specific note on a specific channel.
                 *
                 * @param Pitch The MIDI note number (0-127) to turn off.
                 * @param Vel The release velocity (0-127). Defaults to 0.
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& NoteOff(uint8_t Pitch, uint8_t Vel = 0, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup NoteMessages
                 * @brief Constructs a Note On message.
                 *
                 * Sends a MIDI Note On message to play a specific note on a specific channel.
                 *
                 * @param Pitch The MIDI note number (0-127) to play.
                 * @param Vel The velocity (0-127) of the note. A velocity of 0 is equivalent to a Note Off message.
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& NoteOn(uint8_t Pitch, uint8_t Vel, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup AfterTouch
                 * @brief Constructs an AfterTouch message.
                 *
                 * Sends a MIDI AfterTouch (Polyphonic Key Pressure) message to apply pressure to a specific note.
                 *
                 * @param Note The MIDI note number (0-127) being pressed.
                 * @param Pressure The pressure value (0-127).
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& AfterTouch(uint8_t Note, uint8_t Pressure, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup ControlChange
                 * @brief Constructs a Control Change message.
                 *
                 * Sends a MIDI Control Change (CC) message to modify a controller parameter on a specific channel.
                 *
                 * @param ControllerNumber The MIDI controller number (0-127).
                 * @param Value The value (0-127) for the specified controller.
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& ControlChange(uint8_t ControllerNumber, uint8_t Value, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup ProgramChange
                 * @brief Constructs a Program Change message.
                 *
                 * Sends a MIDI Program Change message to select a specific instrument or program on a specific channel.
                 *
                 * @param Program The program number (0-127) to select.
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& ProgramChange(uint8_t Program, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup ChannelPressure
                 * @brief Constructs a Channel Pressure message.
                 *
                 * Sends a MIDI Channel Pressure (AfterTouch) message to apply pressure across the entire channel.
                 *
                 * @param Pressure The pressure value (0-127).
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& ChannelPressure(uint8_t Pressure, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PitchBend
                 * @brief Constructs a Pitch Bend message using an 8-bit signed value.
                 *
                 * Sends a MIDI Pitch Bend message with a value scaled to the 14-bit pitch bend range.
                 *
                 * @param Value The pitch bend value (-64 to 63). This value is scaled to fit the 14-bit range [-8192, 8191].
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& PitchBend(int8_t Value, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PitchBend
                 * @brief Constructs a Pitch Bend message using a 16-bit signed value.
                 *
                 * Sends a MIDI Pitch Bend message with a value in the 14-bit pitch bend range.
                 *
                 * @param Value The pitch bend value (-8192 to 8191).
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& PitchBend(int16_t Value, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PitchBend
                 * @brief Constructs a Pitch Bend message using an 8-bit unsigned value.
                 *
                 * Sends a MIDI Pitch Bend message with a value scaled to the 14-bit pitch bend range.
                 *
                 * @param Value The pitch bend value (0-127). This value is scaled to fit the 14-bit range [0, 16383].
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& PitchBend(uint8_t Value, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @ingroup PitchBend
                 * @brief Constructs a Pitch Bend message using a 16-bit unsigned value.
                 *
                 * Sends a MIDI Pitch Bend message with a value in the 14-bit pitch bend range.
                 *
                 * @param Value The pitch bend value (0-16383).
                 * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                 * @return A reference to the updated `Message` object.
                 */
                    Message& PitchBend(uint16_t Value, uint8_t Channel = 0);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // MCC_MidiCore::MidiNote

                    #if defined (MIDILAR_MIDI_NOTE)
                        Message& NoteOff(const MidiNote& Source);
                        Message& NoteOn(const MidiNote& Source);
                    #endif
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Mcc_MusicalNote::Note

                    #if defined (MCC_MUSICAL_NOTE_ENABLED)
                        Message& NoteOn(const MusicalNote::Note& Source, uint8_t Vel, uint8_t Channel = 0);
                        Message& NoteOff(const MusicalNote::Note& Source, uint8_t Vel = 0, uint8_t Channel = 0);
                    #endif
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Mcc_MusicalNote::Pitch

                    #if defined (MCC_MUSICAL_NOTE_ENABLED)
                        Message& NoteOn(const MusicalNote::Pitch& Source, uint8_t Octave, uint8_t Vel, uint8_t Channel = 0);
                        Message& NoteOff(const MusicalNote::Pitch& Source, uint8_t Octave, uint8_t Vel = 0, uint8_t Channel = 0);
                    #endif
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name Channel Mode Messages
             * @brief Functions for constructing channel mode messages.
             */
            ///@{

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Emergency messages

                    /**
                     * @brief Sends a MIDI "All Sound Off" message.
                     *
                     * This message stops all sounds on the specified MIDI channel, including sustain pedal sounds,
                     * without affecting the state of the notes or controllers.
                     *
                     * @param Channel The MIDI channel (0-15) on which to stop all sounds.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetAllSoundOff(uint8_t Channel);

                    /**
                     * @brief Sends a MIDI "All Notes Off" message.
                     *
                     * This message turns off all currently playing notes on the specified MIDI channel.
                     * It does not affect sounds triggered by the sustain pedal.
                     *
                     * @param Channel The MIDI channel (0-15) on which to turn off all notes.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetAllNotesOff(uint8_t Channel);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Local Control
                    
                    /**
                     * @brief Sets the Local Control mode using a boolean value.
                     *
                     * This overload provides an intuitive way to enable or disable Local Control.
                     * 
                     * @param mode `true` to enable Local Control, `false` to disable it.
                     * @param channel The MIDI channel to apply the Local Control setting (0-15). 
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetLocalControl(bool mode, uint8_t channel);

                    /**
                     * @brief Sets the Local Control mode using a raw MIDI specification value.
                     *
                     * This overload is useful for low-level users who work directly with MIDI spec values.
                     * Valid values are:
                     * - `MIDI_LOCAL_CONTROL_ON` (127) to enable Local Control.
                     * - `MIDI_LOCAL_CONTROL_OFF` (0) to disable Local Control.
                     *
                     * @param mode The raw MIDI value representing the Local Control mode.
                     * @param channel The MIDI channel to apply the Local Control setting (0-15).
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetLocalControl(uint8_t mode, uint8_t channel);

                    /**
                     * @brief Sets the Local Control mode using the library's `MidiProtocol::LocalControlStatus` enum.
                     *
                     * This overload provides a type-safe way to specify Local Control settings using library-defined enums.
                     * 
                     * @param mode The `MidiProtocol::LocalControlStatus` enum value:
                     *             - `LocalControlStatus::On` to enable Local Control.
                     *             - `LocalControlStatus::Off` to disable Local Control.
                     * @param channel The MIDI channel to apply the Local Control setting (0-15).
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetLocalControl(const MidiProtocol::LocalControlStatus& mode, uint8_t channel);
                    
                    /**
                     * @brief Sends a MIDI message to enable Local Control.
                     *
                     * Local Control allows a MIDI device (e.g., a keyboard) to control its internal sound engine directly.
                     * Enabling Local Control re-establishes this connection.
                     *
                     * @param Channel The MIDI channel (0-15) on which to enable Local Control.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetLocalControlOn(uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to disable Local Control.
                     *
                     * Disabling Local Control disconnects a MIDI device (e.g., a keyboard) from its internal sound engine.
                     * This allows the device to be controlled externally via MIDI.
                     *
                     * @param Channel The MIDI channel (0-15) on which to disable Local Control.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetLocalControlOff(uint8_t Channel);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Omni 
                    
                    /**
                     * @brief Sends a MIDI message to set Omni mode using a raw MIDI value.
                     *
                     * Omni mode determines whether a MIDI device listens to all channels (Omni On)
                     * or only to the specified channel (Omni Off).
                     *
                     * @param Mode A bool representing the Omni mode:
                     *             - 1 to enable Omni mode.
                     *             - 0 to disable Omni mode.
                     * @param Channel The MIDI channel (0-15) on which to set the Omni mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetOmniMode(bool Mode, uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to set Omni mode using a raw MIDI value.
                     *
                     * Omni mode determines whether a MIDI device listens to all channels (Omni On)
                     * or only to the specified channel (Omni Off).
                     *
                     * @param Mode The raw MIDI value representing the Omni mode:
                     *             - `MIDI_OMNI_ON` (127) to enable Omni mode.
                     *             - `MIDI_OMNI_OFF` (0) to disable Omni mode.
                     * @param Channel The MIDI channel (0-15) on which to set the Omni mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetOmniMode(uint8_t Mode, uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to set Omni mode using the library's `MidiProtocol::ChannelMode` enum.
                     *
                     * This overload provides a type-safe way to specify Omni mode settings.
                     *
                     * @param Mode The `MidiProtocol::ChannelMode` enum value:
                     *             - `ChannelMode::OmniOn` to enable Omni mode.
                     *             - `ChannelMode::OmniOff` to disable Omni mode.
                     * @param Channel The MIDI channel (0-15) on which to set the Omni mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetOmniMode(const MidiProtocol::ChannelMode& Mode, uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to enable Omni mode.
                     *
                     * When Omni mode is enabled, the MIDI device listens to messages on all channels.
                     *
                     * @param Channel The MIDI channel (0-15) on which to enable Omni mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetOmniOn(uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to disable Omni mode.
                     *
                     * When Omni mode is disabled, the MIDI device listens only to messages on the specified channel.
                     *
                     * @param Channel The MIDI channel (0-15) on which to disable Omni mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetOmniOff(uint8_t Channel);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Polyphony
                
                    /**
                     * @brief Sends a MIDI message to set the Polyphony mode using a raw MIDI value.
                     *
                     * Polyphony mode determines whether the MIDI device operates in Mono mode 
                     * (one note at a time) or Polyphonic mode (multiple notes simultaneously).
                     *
                     * @param Mode A bool representing the Polyphony mode:
                     *             - 0 to enable Mono mode.
                     *             - 1 to enable Polyphonic mode.
                     * @param Channel The MIDI channel (0-15) on which to set the Polyphony mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetPolyphony(bool Mode, uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to set the Polyphony mode using a raw MIDI value.
                     *
                     * Polyphony mode determines whether the MIDI device operates in Mono mode 
                     * (one note at a time) or Polyphonic mode (multiple notes simultaneously).
                     *
                     * @param Mode The raw MIDI value representing the Polyphony mode:
                     *             - `MIDI_MONO` (0x7E) to enable Mono mode.
                     *             - `MIDI_POLY` (0x7F) to enable Polyphonic mode.
                     * @param Channel The MIDI channel (0-15) on which to set the Polyphony mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetPolyphony(uint8_t Mode, uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to set the Polyphony mode using the library's `MidiProtocol::ChannelMode` enum.
                     *
                     * This overload provides a type-safe way to specify the Polyphony mode.
                     *
                     * @param Mode The `MidiProtocol::ChannelMode` enum value:
                     *             - `ChannelMode::Mono` to enable Mono mode.
                     *             - `ChannelMode::Poly` to enable Polyphonic mode.
                     * @param Channel The MIDI channel (0-15) on which to set the Polyphony mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetPolyphony(const MidiProtocol::ChannelMode& Mode, uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to enable Mono mode.
                     *
                     * When Mono mode is enabled, the MIDI device can only play one note at a time on the specified channel.
                     *
                     * @param Channel The MIDI channel (0-15) on which to enable Mono mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetMono(uint8_t Channel);

                    /**
                     * @brief Sends a MIDI message to enable Polyphonic mode.
                     *
                     * When Polyphonic mode is enabled, the MIDI device can play multiple notes simultaneously on the specified channel.
                     *
                     * @param Channel The MIDI channel (0-15) on which to enable Polyphonic mode.
                     *                Values outside this range will be clamped.
                     * @return A reference to the updated `Message` object.
                     */
                    Message& SetPoly(uint8_t Channel);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name System Common Messages
             * @brief Functions for constructing system common messages.
             */
            ///@{
                
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Midi Time Code

                    Message& MTC_QuarterFrame(uint8_t TimeComponent, uint8_t Data); ///< Constructs an MTC Quarter Frame message.
                    Message& MTC_QuarterFrame(const MidiProtocol_MTC::TimeComponent& TimeComponent, uint8_t Data); ///< Constructs an MTC Quarter Frame message.

                    Message& MTC_FullFrame(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Frames, uint8_t Framerate, uint8_t SysexChannel = 0x7F);
                    Message& MTC_FullFrame(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Frames, const MidiProtocol_MTC::FrameRate& Framerate, uint8_t SysexChannel = 0x7F);
                //
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                Message& SongPositionPointer(uint16_t Position); ///< Constructs a Song Position Pointer message.
                Message& SongSelect(uint8_t Song); ///< Constructs a Song Select message.
                Message& TuningRequest(); ///< Constructs a Tuning Request message.
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name System Real Time Messages
             * @brief Functions for constructing System Real Time Messages.
             */
            ///@{

                Message& TimingTick(); ///< Constructs a Timing Tick message.
                Message& Start(); ///< Constructs a Start message.
                Message& Continue(); ///< Constructs a Continue message.
                Message& Stop(); ///< Constructs a Stop message.
                Message& ActiveSensing(); ///< Constructs an Active Sensing message.
                Message& SystemReset(); ///< Constructs a System Reset message.
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name System Exclusive Messages
             * @brief Functions for constructing System Exclusive Messages.
             */
            ///@{
                
                Message& SystemExclusive(uint8_t* Data, uint8_t Length); ///< Constructs a System Exclusive message.

                #if __has_include(<vector>)
                    Message& SystemExclusive(const std::vector<uint8_t>& Data); ///< Constructs a System Exclusive message from a vector.
                #endif
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name NRPN and RPN Messages
             * @brief Functions for constructing NRPN and RPN Messages.
             */
            ///@{

                Message& NRPN(uint16_t ParameterID, uint8_t Data); ///< Constructs an NRPN message with an 8-bit value.
                Message& NRPN(uint16_t ParameterID, uint16_t Data); ///< Constructs an NRPN message with a 16-bit value.
                Message& NRPN_DataIncrement(); ///< Increments NRPN data.
                Message& NRPN_DataDecrement(); ///< Decrements NRPN data.
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            private:

                bool _resize(size_t new_size); ///< Resizes the internal buffer for MIDI data.
            
        };


    }



#endif//MIDILAR_MIDI_MESSAGE_H