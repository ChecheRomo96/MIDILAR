#ifndef MIDILAR_MIDI_MESSAGE_H
#define MIDILAR_MIDI_MESSAGE_H

/**
 * @file MIDILAR_MidiMessage.h
 * @brief Provides the `Message` class for creating and managing MIDI messages.
 */

    #include <System/BuildSettings.h>
    #include <stdint.h>

    #if __has_include(<vector>)
        #include <vector>
    #endif

    #include <MidiFoundation/Protocol/Defines.h>
    #include <MidiFoundation/Protocol/Enums.h>

    namespace MIDILAR::MidiFoundation{
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @class Message
         * @brief Represents a MIDI message for communication between MIDI devices.
         * 
         * The `Message` class provides a flexible and efficient way to construct, 
         * modify, and transmit MIDI messages. It supports various types of MIDI 
         * messages, including:
         * 
         * - **Channel Voice Messages** (Note On, Note Off, Control Change, Program Change, etc.).
         * - **System Common Messages** (Song Position Pointer, Song Select, Tuning Request, etc.).
         * - **System Real-Time Messages** (Clock, Start, Stop, Continue, Active Sensing).
         * - **System Exclusive (SysEx) Messages** for manufacturer-specific communication.
         * 
         * ## Features:
         * - Construct and parse standard MIDI messages.
         * - Efficient memory usage for real-time performance.
         * - Support for both short (status + data bytes) and long (SysEx) messages.
         * - Integration with `MidiFoundation` utilities for seamless MIDI event handling.
         * 
         * ## Example Usage:
         * ```cpp
         * Message msg;
         * msg.NoteOn(0, 60, 127); // Channel 0, Middle C, Max Velocity
         * ```
         * 
         * @note This class is designed to work efficiently on embedded and real-time 
         *       MIDI processing environments.
         * 
         * @see MIDILAR::MidiFoundation::ChannelVoiceMessages
         * @see MIDILAR::MidiFoundation::SystemMessages
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
                * @{
                */
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
               /**@}*/ //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /** 
                * @name Data Access
                * @{
                */
                    const uint8_t* Buffer() const; ///< Returns a pointer to the first data element stored in the message
                    uint8_t Data(size_t index) const; ///< Returns the data stored at the specified index
                    size_t size() const;  ///< Returns the size of the message stored in the object.  

                    Message& SetRawData(const uint8_t* Data, size_t Size);
               
                    #if __has_include(<vector>)
                     Message& SetRawData(const std::vector<uint8_t>& Data);
                    #endif  

               /**@}*/ //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /** 
                * @name Vector API
                * @brief Set of functions to access the data when std::vector is available.
                * This set of functions does not replace the Data Access group of functions but adds on to enhance the member data accessibility.
                * @{
                */
                    #if __has_include(<vector>)
                        const std::vector<uint8_t>& Vector() const; ///< Gets the internal data buffer.
                        std::vector<uint8_t>::iterator begin() noexcept; ///< Gets an iterator to the beginning of the buffer.
                        std::vector<uint8_t>::const_iterator cbegin() const noexcept; ///< Gets a constant iterator to the beginning of the buffer.
                        std::vector<uint8_t>::iterator end() noexcept; ///< Gets an iterator to the end of the buffer.
                        std::vector<uint8_t>::const_iterator cend() const noexcept; ///< Gets a constant iterator to the end of the buffer.
                    #endif  
               /**@}*/ //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /** 
                * @name MIDILAR::MidiFoundation::Message Channel Voice Messages
                * @brief Functions for constructing and handling MIDI Channel Voice messages.
                * 
                * MIDI Channel Voice messages are fundamental to MIDI communication, allowing 
                * control over musical performance parameters such as note triggering, velocity, 
                * control changes, and program selection. These functions facilitate the 
                * construction and transmission of these messages.
                * 
                * These functions provide low-level access to MIDI channel voice messages, ensuring 
                * efficient communication between MIDI controllers, synthesizers, and digital audio workstations (DAWs).
                * @{
                */

                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Note Messages

                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     // NoteOff
                     /**
                      * @ingroup MIDI_CV_NoteMessages
                      * @brief Constructs a Note Off message.
                      *
                      * Sends a MIDI Note Off message to stop a specific note on a specific channel.
                      *
                      * @param Pitch The MIDI note number (0-127) to turn off.
                      * @param Vel The release velocity (0-127). Defaults to 0.
                      * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                      * @return A reference to the updated `Message` object.
                      * @{
                      */
                         Message& NoteOff(uint8_t Pitch, uint8_t Vel = 0, uint8_t Channel = 0);
                     /**@}*/
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     // NoteOn
                     /**
                      * @ingroup MIDI_CV_NoteMessages
                      * @brief Constructs a Note On message.
                      *
                      * Sends a MIDI Note On message to play a specific note on a specific channel.
                      *
                      * @param Pitch The MIDI note number (0-127) to play.
                      * @param Vel The velocity (0-127) of the note. A velocity of 0 is equivalent to a Note Off message.
                      * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                      * @return A reference to the updated `Message` object.
                      * @{
                      */
                         Message& NoteOn(uint8_t Pitch, uint8_t Vel, uint8_t Channel = 0);
                     /**@}*/
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // After Touch
                  /**
                   * @ingroup MIDI_CV_AfterTouch
                   * @memberof Message
                   * @brief Constructs an AfterTouch message.
                   *
                   * Sends a MIDI AfterTouch (Polyphonic Key Pressure) message to apply pressure to a specific note.
                   *
                   * @param Note The MIDI note number (0-127) being pressed.
                   * @param Pressure The pressure value (0-127).
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& AfterTouch(uint8_t Note, uint8_t Pressure, uint8_t Channel = 0);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Program Change
                  /**
                   * @ingroup MIDI_CV_ProgramChange
                   * @brief Constructs a Program Change message.
                   *
                   * Sends a MIDI Program Change message to select a new instrument or preset on the specified channel.
                   *
                   * @param Program The program (instrument) number (0-127).
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& ProgramChange(uint8_t Program, uint8_t Channel);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Channel Pressure
                  /**
                   * @ingroup ChannelPressure
                   * @brief Constructs a Channel Pressure message.
                   *
                   * Sends a MIDI Channel Pressure (AfterTouch) message to apply pressure across the entire channel.
                   *
                   * @param Pressure The pressure value (0-127).
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& ChannelPressure(uint8_t Pressure, uint8_t Channel = 0);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Pitch Bend int8_t
                  /**
                   * @ingroup PitchBend
                   * @brief Constructs a Pitch Bend message using an 8-bit signed value.
                   *
                   * Sends a MIDI Pitch Bend message with a value scaled to the 14-bit pitch bend range.
                   *
                   * @param Value The pitch bend value (-64 to 63). This value is scaled to fit the 14-bit range [-8192, 8191].
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& PitchBend(int8_t Value, uint8_t Channel = 0);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Pitch Bend int16_t
                  /**
                   * @ingroup PitchBend
                   * @brief Constructs a Pitch Bend message using a 16-bit signed value.
                   *
                   * Sends a MIDI Pitch Bend message with a value in the 14-bit pitch bend range.
                   *
                   * @param Value The pitch bend value (-8192 to 8191).
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& PitchBend(int16_t Value, uint8_t Channel = 0);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Pitch Bend uint8_t
                  /**
                   * @ingroup PitchBend
                   * @brief Constructs a Pitch Bend message using an 8-bit unsigned value.
                   *
                   * Sends a MIDI Pitch Bend message with a value scaled to the 14-bit pitch bend range.
                   *
                   * @param Value The pitch bend value (0-127). This value is scaled to fit the 14-bit range [0, 16383].
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& PitchBend(uint8_t Value, uint8_t Channel = 0);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Pitch Bend uint16_t
                  /**
                   * @ingroup PitchBend
                   * @brief Constructs a Pitch Bend message using a 16-bit unsigned value.
                   *
                   * Sends a MIDI Pitch Bend message with a value in the 14-bit pitch bend range.
                   * 
                   * @param Value The pitch bend value (0-16383).
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& PitchBend(uint16_t Value, uint8_t Channel = 0);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Control Change
                  /**
                   * @ingroup MIDI_CV_ControlChange
                   * @brief Constructs a Control Change message.
                   *
                   * Sends a MIDI Control Change (CC) message to modify a controller parameter on a specific channel.
                   *
                   * @param ControllerNumber The MIDI controller number (0-127).
                   * @param Value The value (0-127) for the specified controller.
                   * @param Channel The MIDI channel (0-15). Defaults to 0. Values outside this range will be clamped.
                   * @return A reference to the updated `Message` object.
                   * @{
                   */
                     Message& ControlChange(uint8_t ControllerNumber, uint8_t Value, uint8_t Channel = 0);
                     
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     // General CC Messages

                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Bank Select
                         /**
                          * @brief Sets the Bank Select value.
                          *
                          * Bank Select is used to switch between different banks of sounds or settings.
                          *
                          * @param Value The bank select value (0-127).
                          * @param Channel The MIDI channel to apply the Bank Select setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_BankSelect(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Modulation
                         /**
                          * @brief Sets the Modulation value.
                          *
                          * Modulation is commonly used to add vibrato or other expressive effects to a sound.
                          *
                          * @param Value The modulation depth (0-127).
                          * @param Channel The MIDI channel to apply the modulation (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_Modulation(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Breath Control
                         /**
                          * @brief Sets the Breath Control value.
                          *
                          * Breath Control is often used with wind controllers or to simulate breath pressure on synthesizers.
                          *
                          * @param Value The breath control value (0-127).
                          * @param Channel The MIDI channel to apply the Breath Control setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_BreathControl(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Foot Pedal
                         /**
                          * @brief Sets the Foot Pedal (Control Change 4) value.
                          *
                          * This control is typically used to simulate sustain or other foot-controlled effects.
                          *
                          * @param Value The foot pedal value (0-127).
                          * @param Channel The MIDI channel to apply the Foot Pedal setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_FootPedal(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Portamento
                         /**
                          * @brief Sets the Portamento Time value.
                          *
                          * Portamento controls the time it takes to slide between notes.
                          *
                          * @param Value The portamento time value (0-127).
                          * @param Channel The MIDI channel to apply the Portamento setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_Portamento(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Volume
                         /**
                          * @brief Sets the Volume level.
                          *
                          * Controls the overall volume of the instrument.
                          *
                          * @param Value The volume level (0-127).
                          * @param Channel The MIDI channel to apply the Volume setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_Volume(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Balance
                         /**
                          * @brief Sets the Balance control.
                          *
                          * Balance adjusts the level between left and right channels.
                          *
                          * @param Value The balance value (0-127).
                          * @param Channel The MIDI channel to apply the Balance setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_Balance(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Pan
                         /**
                          * @brief Sets the Pan position.
                          *
                          * Pan controls the stereo position of the sound.
                          *
                          * @param Value The pan position (0-127), where 0 is fully left, 127 is fully right, and 64 is center.
                          * @param Channel The MIDI channel to apply the Pan setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_Pan(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Expression
                         /**
                          * @brief Sets the Expression control value.
                          *
                          * Expression is a secondary volume control that allows dynamic changes to be applied over time.
                          *
                          * @param Value The expression value (0-127).
                          * @param Channel The MIDI channel to apply the Expression setting (0-15). 
                          *                Values outside this range will be clamped.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_Expression(uint8_t Value, uint8_t Channel=0);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                     //
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     // Channel Mode Messages

                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // All Sound Off
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
                             Message& CC_AllSoundOff(uint8_t Channel);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // All Notes Off
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
                             Message& CC_AllNotesOff(uint8_t Channel);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Local Control
                             
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // CC_LocalControl(bool Mode, uint8_t Channel)
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
                                 Message& CC_LocalControl(bool Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_LocalControl(uint8_t Mode, uint8_t Channel)
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
                                 Message& CC_LocalControl(uint8_t Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // CC_LocalControl(const MidiProtocol::LocalControlStatus& Mode, uint8_t Channel)
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
                                 Message& CC_LocalControl(const MidiProtocol::LocalControlStatus& Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // CC_LocalControlOn(uint8_t Channel)
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
                                 Message& CC_LocalControlOn(uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // CC_LocalControlOff(uint8_t Channel)
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
                                 Message& CC_LocalControlOff(uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Omni 
                             
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_OmniMode(bool Mode, uint8_t Channel);
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
                                 Message& CC_OmniMode(bool Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_OmniMode(uint8_t Mode, uint8_t Channel);
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
                                 Message& CC_OmniMode(uint8_t Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_OmniMode(const MidiProtocol::ChannelMode& Mode, uint8_t Channel);
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
                                 Message& CC_OmniMode(const MidiProtocol::ChannelMode& Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // CC_OmniOn(uint8_t Channel);
                             /**
                              * @brief Sends a MIDI message to enable Omni mode.
                              *
                              * When Omni mode is enabled, the MIDI device listens to messages on all channels.
                              *
                              * @param Channel The MIDI channel (0-15) on which to enable Omni mode.
                              *                Values outside this range will be clamped.
                              * @return A reference to the updated `Message` object.
                              */
                                 Message& CC_OmniOn(uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // CC_OmniOff(uint8_t Channel);
                             /**
                              * @brief Sends a MIDI message to disable Omni mode.
                              *
                              * When Omni mode is disabled, the MIDI device listens only to messages on the specified channel.
                              *
                              * @param Channel The MIDI channel (0-15) on which to disable Omni mode.
                              *                Values outside this range will be clamped.
                              * @return A reference to the updated `Message` object.
                              */
                                 Message& CC_OmniOff(uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // Polyphony
                         
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_Polyphony(bool Mode, uint8_t Channel);
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
                                 Message& CC_Polyphony(bool Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_Polyphony(uint8_t Mode, uint8_t Channel);
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
                                 Message& CC_Polyphony(uint8_t Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_Polyphony(const MidiProtocol::ChannelMode& Mode, uint8_t Channel);
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
                                 Message& CC_Polyphony(const MidiProtocol::ChannelMode& Mode, uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_Mono(uint8_t Channel);
                             /**
                              * @brief Sends a MIDI message to enable Mono mode.
                              *
                              * When Mono mode is enabled, the MIDI device can only play one note at a time on the specified channel.
                              *
                              * @param Channel The MIDI channel (0-15) on which to enable Mono mode.
                              *                Values outside this range will be clamped.
                              * @return A reference to the updated `Message` object.
                              */
                                 Message& CC_Mono(uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // Message& CC_Poly(uint8_t Channel);
                             /**
                              * @brief Sends a MIDI message to enable Polyphonic mode.
                              *
                              * When Polyphonic mode is enabled, the MIDI device can play multiple notes simultaneously on the specified channel.
                              *
                              * @param Channel The MIDI channel (0-15) on which to enable Polyphonic mode.
                              *                Values outside this range will be clamped.
                              * @return A reference to the updated `Message` object.
                              */
                                 Message& CC_Poly(uint8_t Channel);
                             //
                             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         //
                         //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
                     //
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     // RPN & NRPN Messages

                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // CC_NRPN(uint16_t ParameterID, uint8_t Data);
                         /**
                          * @brief Sends a Non-Registered Parameter Number (NRPN) message with an 8-bit data value.
                          *
                          * NRPNs allow for manufacturer-specific or extended MIDI control beyond standard CC messages.
                          *
                          * @param ParameterID The 14-bit NRPN parameter identifier (0-16383).
                          * @param Data The 8-bit data value (0-127) to send for the specified NRPN parameter.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_NRPN(uint16_t ParameterID, uint8_t Data);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // CC_NRPN(uint16_t ParameterID, uint16_t Data);
                         /**
                          * @brief Sends a Non-Registered Parameter Number (NRPN) message with a 16-bit data value.
                          *
                          * This overload allows sending NRPN messages that require a full 14-bit data value.
                          *
                          * @param ParameterID The 14-bit NRPN parameter identifier (0-16383).
                          * @param Data The 14-bit data value (0-16383) to send for the specified NRPN parameter.
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_NRPN(uint16_t ParameterID, uint16_t Data);
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // CC_NRPN_DataIncrement();
                         /**
                          * @brief Sends an NRPN Data Increment message.
                          *
                          * This message increases the value of the currently selected NRPN parameter by 1.
                          * It is typically used for relative control of parameters.
                          *
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_NRPN_DataIncrement();
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                         // CC_NRPN_DataDecrement();
                         /**
                          * @brief Sends an NRPN Data Decrement message.
                          *
                          * This message decreases the value of the currently selected NRPN parameter by 1.
                          * It is typically used for relative control of parameters.
                          *
                          * @return A reference to the updated `Message` object.
                          */
                             Message& CC_NRPN_DataDecrement();
                         //
                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     //
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // MCC_MidiCore::MidiNote

                     #if defined (MIDILAR_MIDI_NOTE)
                        Message& NoteOff(const MidiNote& Source);
                        Message& NoteOn(const MidiNote& Source);
                     #endif
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**@}*/ //End of Channel Voice
               ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /** 
                * @name Real Time Messages
                * @brief Functions for constructing real time messages.
                * @{
                */

                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Timing Tick
                  /**
                   * @ingroup MIDI_RT_TimingTick
                   * @brief Constructs a MIDI Timing Tick (0xF8) message.
                   * 
                   * The Timing Tick message is sent 24 times per quarter note to synchronize tempo between MIDI devices.
                   * It is used in MIDI clock-based synchronization to ensure precise timing.
                   * 
                   * @return Reference to the modified Message object.
                   */
                     Message& TimingTick();
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Start
                  /**
                   * @ingroup MIDI_RT_Start
                   * @brief Constructs a MIDI Start (0xFA) message.
                   * 
                   * The Start message instructs all connected MIDI devices to begin playback from the beginning of a sequence.
                   * It resets the song position to zero before playback starts.
                   * 
                   * @return Reference to the modified Message object.
                   */    
                     Message& Start();
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Continue
                  /**
                   * @ingroup MIDI_RT_Continue
                   * @brief Constructs a MIDI Continue (0xFB) message.
                   * 
                   * The Continue message resumes playback from the current song position without resetting it.
                   * This is useful when playback has been paused and needs to resume without restarting.
                   * 
                   * @return Reference to the modified Message object.
                   */
                     Message& Continue();
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Stop
                  /**
                   * @ingroup MIDI_RT_Stop
                   * @brief Constructs a MIDI Stop (0xFC) message.
                   * 
                   * The Stop message instructs all connected MIDI devices to halt playback immediately.
                   * It does not reset the song position, allowing playback to resume later using the Continue message.
                   * 
                   * @return Reference to the modified Message object.
                   */
                     Message& Stop();
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Active Sensing
                  /**
                   * @ingroup MIDI_RT_ActiveSensing
                   * @brief Constructs a MIDI Active Sensing (0xFE) message.
                   * 
                   * The Active Sensing message is periodically sent to indicate an active MIDI connection.
                   * If a device stops receiving these messages, it may assume the connection has been lost and reset itself.
                   * 
                   * @return Reference to the modified Message object.
                   */
                     Message& ActiveSensing();
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // System Reset
                  /**
                   * @ingroup MIDI_RT_SystemReset
                   * @brief Constructs a MIDI System Reset (0xFF) message.
                   * 
                   * The System Reset message forces all connected MIDI devices to reset to their default state.
                   * It is typically used for error recovery or to restore a known system state.
                   * 
                   * @return Reference to the modified Message object.
                   */
                     Message& SystemReset();
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**@}*/ //End of Real Time
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /** 
                * @name System Common Messages
                * @brief Functions for constructing system common messages.
                * @{
                */

                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Midi Time Code Quarter Frame Message

                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     /**
                      * @ingroup MTC_QuarterFrame
                      * @brief Constructs an MTC Quarter Frame message.
                      * 
                      * @param TimeComponent The time component of the MTC Quarter Frame message, represented as a `uint8_t`.
                      * @param Data The data value associated with the time component.
                      * @return Reference to the constructed `Message` object.
                      *@{
                      */
                         Message& MTC_QuarterFrame(uint8_t TimeComponent, uint8_t Data); ///< Constructs an MTC Quarter Frame message.
                     /**@}*/
                     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     /**
                      * @ingroup MTC_QuarterFrame
                      * @brief Constructs an MTC Quarter Frame message.
                      * 
                      * @param TimeComponent The time component of the MTC Quarter Frame message, represented as an instance of `MidiProtocol_MTC::TimeComponent`.
                      * @param Data The data value associated with the time component.
                      * @return Reference to the constructed `Message` object.
                      *@{
                      */
                         Message& MTC_QuarterFrame(const MidiProtocol_MTC::TimeComponent& TimeComponent, uint8_t Data); ///< Constructs an MTC Quarter Frame message.
                     /**@}*/
                     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Song Position Pointer
                  /**
                   * @brief Constructs a Song Position Pointer (SPP) message.
                   * 
                   * The Song Position Pointer (SPP) message is a MIDI System Common message 
                   * that specifies the current position within a song in terms of MIDI beats 
                   * (sixteenth-note divisions). This allows sequencers, drum machines, and 
                   * other MIDI devices to synchronize playback accurately.
                   * 
                   * ## Message Format
                   * The SPP message consists of three bytes:
                   * 
                   * | Byte | Value                  | Description |
                   * |------|------------------------|-------------|
                   * | 0xF2 | Song Position Pointer  | Status byte |
                   * | LSB  | `Position & 0x7F`      | Least significant 7 bits |
                   * | MSB  | `(Position >> 7) & 0x7F` | Most significant 7 bits |
                   * 
                   * @param Position The song position in MIDI beats (0-16383). Each beat represents 
                   *                 a sixteenth note.
                   * @return Reference to the modified Message object.
                   * 
                   * @note This message is typically used for syncing MIDI devices during playback.
                   * @see SongSelect()
                   *@{
                   */
                     Message& SongPositionPointer(uint16_t Position);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Song Select
                  /**
                   * @brief Constructs a Song Select message.
                   * 
                   * The Song Select message is a MIDI System Common message used to select a 
                   * song or sequence number in a MIDI device, such as a sequencer or synthesizer.
                   * 
                   * ## Message Format
                   * The Song Select message consists of two bytes:
                   * 
                   * | Byte | Value          | Description |
                   * |------|--------------|-------------|
                   * | 0xF3 | Song Select  | Status byte |
                   * | Song | `0x00 - 0x7F` | Song number (0-127) |
                   * 
                   * @param Song The song number to be selected (0-127).
                   * @return Reference to the modified Message object.
                   * 
                   * @note The meaning of the song number depends on the receiving device's internal implementation.
                   * @see SongPositionPointer()
                   *@{
                   */
                     Message& SongSelect(uint8_t Song);
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Tunning Request
                  /**
                   * @brief Constructs a Tuning Request message.
                   * 
                   * The Tuning Request message is a MIDI System Common message used to request 
                   * real-time tuning adjustments for analog synthesizers and other MIDI devices 
                   * that support microtonal or fine-tuning capabilities.
                   * 
                   * ## Message Format
                   * The Tuning Request message consists of a single byte:
                   * 
                   * | Byte | Value         | Description |
                   * |------|-------------|-------------|
                   * | 0xF6 | Tuning Request | Status byte |
                   * 
                   * @return Reference to the modified Message object.
                   * 
                   * @note This message is typically used in conjunction with MIDI tuning standards 
                   *       such as MIDI Tuning Standard (MTS) for microtonal adjustments.
                   *@{
                   */
                     Message& TuningRequest();
                  /**@}*/
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**@}*/ //End of System Common
               ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /** 
                * @name System Exclusive Messages
                * @brief Functions for constructing System Exclusive Messages.
                * @{
                */
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // Message& SystemExclusive(uint8_t* Data, uint8_t Length);
                  /**
                   * @brief Constructs a System Exclusive (SysEx) message.
                   * 
                   * This function constructs a System Exclusive (SysEx) MIDI message using 
                   * raw byte data. SysEx messages allow for manufacturer-specific or extended 
                   * MIDI functionality beyond the standard protocol.
                   * 
                   * @param Data Pointer to the raw SysEx message data.
                   * @param Length The number of bytes in the SysEx message.
                   * @return Reference to the modified Message object.
                   */
                     Message& SystemExclusive(uint8_t* Data, uint8_t Length); ///< Constructs a System Exclusive message.
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // SystemExclusive(const std::vector<uint8_t>& Data);

                     #if __has_include(<vector>)
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        /**
                         * @brief Constructs a System Exclusive (SysEx) message from a vector.
                         * 
                         * This function constructs a System Exclusive (SysEx) MIDI message using a 
                         * `std::vector<uint8_t>`, which allows for dynamic and safe storage of SysEx data.
                         * 
                         * @param Data A vector containing the SysEx message bytes.
                         * @return Reference to the modified Message object.
                         */
                           Message& SystemExclusive(const std::vector<uint8_t>& Data);
                        //
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     #endif
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // MTC Full frame
                           
                     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     /**
                      * @ingroup MTC_RT_FullFrame
                      * @ingroup MTC_FrameRates
                      * @brief Constructs an MTC Full Frame message.
                      * 
                      * This function creates a Full Frame MIDI Time Code (MTC) System Exclusive message.
                      * MTC Full Frame messages are used for absolute time synchronization in hours, 
                      * minutes, seconds, and frames.
                      * 
                      * @param Hours The hour component of the timestamp (0-23).
                      * @param Minutes The minute component of the timestamp (0-59).
                      * @param Seconds The second component of the timestamp (0-59).
                      * @param Frames The frame component of the timestamp (0-29 depending on the frame rate).
                      * @param Framerate The frame rate (encoded in 2 bits), indicating 24, 25, 30 drop-frame, or 30 non-drop-frame.
                      * @param SysexChannel The System Exclusive channel (default: 0x7F, which means "all devices").
                      * @return Reference to the modified Message object.
                      */
                        
                        Message& MTC_FullFrame( uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Frames, uint8_t Framerate, uint8_t SysexChannel = 0x7F);
                     //
                     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     /**
                      * @ingroup MTC_RT_FullFrame
                      * @ingroup MTC_FrameRates
                      * @brief Constructs an MTC Full Frame message with an explicit FrameRate type.
                      * 
                      * This overload of `MTC_FullFrame` constructs a Full Frame MIDI Time Code (MTC) 
                      * System Exclusive message using a `MidiProtocol_MTC::FrameRate` type instead 
                      * of a raw `uint8_t` for frame rate.
                      * 
                      * @param Hours The hour component of the timestamp (0-23).
                      * @param Minutes The minute component of the timestamp (0-59).
                      * @param Seconds The second component of the timestamp (0-59).
                      * @param Frames The frame component of the timestamp (0-29 depending on the frame rate).
                      * @param Framerate The frame rate specified using `MidiProtocol_MTC::FrameRate`.
                      * @param SysexChannel The System Exclusive channel (default: 0x7F, which means "all devices").
                      * @return Reference to the modified Message object.
                      */
                        Message& MTC_FullFrame(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Frames, const MidiProtocol_MTC::FrameRate& Framerate, uint8_t SysexChannel = 0x7F);
                     //
                     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  //
                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**@}*/ //
               ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                private:

                    bool _resize(size_t new_size); ///< Resizes the internal buffer for MIDI data.
                
            };
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    }



#endif//MIDILAR_MIDI_MESSAGE_H