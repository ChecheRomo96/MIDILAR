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
                Message(char* Buffer, uint8_t size);        ///< Constructor from a raw character buffer.
                
                #if __has_include(<vector>)
                    Message(const std::vector<uint8_t>& MessageBuffer);
                    Message(std::vector<uint8_t>&& MessageBuffer) noexcept;
                #endif
                
                Message& operator=(const Message& Source);
                Message& operator=(Message&& Source) noexcept;

                #if __has_include(<vector>)
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
            /** 
             * @name ChannelVoice Channel Voice Messages
             * @brief Functions for constructing channel voice messages.
             */
            ///@{
                
                Message& NoteOff(uint8_t Pitch, uint8_t Vel = 0, uint8_t Channel = 0); ///< Constructs a Note Off message.
                Message& NoteOn(uint8_t Pitch, uint8_t Vel, uint8_t Channel = 0); ///< Constructs a Note On message.
                Message& AfterTouch(uint8_t Note, uint8_t Pressure, uint8_t Channel = 0); ///< Constructs an AfterTouch message.
                Message& ControlChange(uint8_t ControllerNumber, uint8_t Value, uint8_t Channel = 0); ///< Constructs a Control Change message.
                Message& ProgramChange(uint8_t Program, uint8_t Channel = 0); ///< Constructs a Program Change message.
                Message& ChannelPressure(uint8_t Pressure, uint8_t Channel = 0); ///< Constructs a Channel Pressure message.
                Message& PitchBend(int8_t Value, uint8_t Channel = 0); ///< Constructs a Pitch Bend message (8-bit value).
                Message& PitchBend(int16_t Value, uint8_t Channel = 0); ///< Constructs a Pitch Bend message (16-bit signed value).
                Message& PitchBend(uint8_t Value, uint8_t Channel = 0); ///< Constructs a Pitch Bend message (8-bit unsigned value).
                Message& PitchBend(uint16_t Value, uint8_t Channel = 0); ///< Constructs a Pitch Bend message (16-bit unsigned value).

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
       
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name Channel Mode Messages
             * @brief Functions for constructing channel mode messages.
             */
            ///@{

                Message& SetLocalControl(uint8_t Mode); ///< Sets the Local Control mode.
                Message& SetLocalControlOn(); ///< Enables Local Control.
                Message& SetLocalControlOff(); ///< Disables Local Control.
                Message& SetAllSoundOff(); ///< Turns off all sounds.
                Message& SetAllNotesOff(); ///< Turns off all notes.
                Message& SetChannelMode(uint8_t Omni, uint8_t Poly); ///< Sets the channel mode (Omni/Poly).
                Message& SetChannelMode_Omni(uint8_t Mode); ///< Sets Omni mode.
                Message& SetChannelMode_OmniOn(); ///< Enables Omni mode.
                Message& SetChannelMode_OmniOff(); ///< Disables Omni mode.
                Message& SetChannelMode_Polyphony(uint8_t Mode); ///< Sets Polyphony mode.
                Message& SetChannelMode_Mono(); ///< Enables Mono mode.
                Message& SetChannelMode_Poly(); ///< Enables Polyphonic mode.
            ///@}
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 
             * @name System Common Messages
             * @brief Functions for constructing system common messages.
             */
            ///@{

                Message& MTC_QuarterFrame(uint8_t MTC_ID, uint8_t Data); ///< Constructs an MTC Quarter Frame message.
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