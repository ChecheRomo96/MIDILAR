#include "Message.h"
#include "Protocol/Defines.h"

namespace MIDILAR::MidiFoundation{

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private resize method

        bool Message::_resize(size_t new_size) {
            #if __has_include(<vector>)
                if (new_size <= _Data.size()) {
                    return true; // No need to resize if the current size is already sufficient
                }

                try {
                    _Data.resize(new_size);
                    return true; // Resizing successful
                } catch (const std::bad_alloc&) {
                    return false; // Allocation failed
                }
                
            #else
                if (new_size <= _BufferSize) {
                    return true; // No need to resize if the current size is already sufficient
                }

                uint8_t* new_data = (uint8_t*)calloc(new_size, sizeof(uint8_t));
                if (new_data == nullptr) {
                    return false; // Memory allocation failed
                }

                // Copy existing data to the newly allocated memory, if applicable
                if (_Data != nullptr) {
                    size_t copy_size = (_BufferSize < new_size) ? _BufferSize : new_size;
                    for (size_t i = 0; i < copy_size; ++i) {
                        new_data[i] = _Data[i];
                    }
                    free(_Data); // Release the old memory
                }

                _Data = new_data;
                _BufferSize = new_size;
                return true; // Successful resize
            #endif
        }
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors and Assignment Operators

        Message::Message() noexcept{
            #if __has_include(<vector>)
                // std::vector is automatically initialized
            #else
                _Data = nullptr;
                _BufferSize = 0;
                _MessageSize = 0;
            #endif
        }

        Message::Message(const Message& MessageBuffer) {
            #if __has_include(<vector>)
                // For vector, we just copy the vector data
                _Data = MessageBuffer._Data;  // This works because std::vector handles its own memory management
            #else
                // Resize the buffer to accommodate the new message size
                if (!_resize(MessageBuffer._MessageSize)) {
                    // If resizing fails, handle the failure (e.g., throw an exception, or set a failure flag)
                    return;
                }

                _BufferSize = MessageBuffer._MessageSize;
                _MessageSize = MessageBuffer._MessageSize;

                // Copy the data byte by byte
                for (size_t i = 0; i < _MessageSize; ++i) {
                    _Data[i] = MessageBuffer._Data[i];
                }
            #endif
        }

        Message::Message(Message&& MessageBuffer) noexcept {
            #if __has_include(<vector>)
                // For vector, the move constructor is automatically handled by std::vector
                _Data = std::move(MessageBuffer._Data);  // std::move transfers ownership of the vector
            #else
                // Move the data pointer and update size variables
                _Data = MessageBuffer._Data;  // Take ownership of the existing buffer
                _BufferSize = MessageBuffer._BufferSize;
                _MessageSize = MessageBuffer._MessageSize;

                // Leave the source object in a valid state (set it to default or null)
                MessageBuffer._Data = nullptr;
                MessageBuffer._BufferSize = 0;
                MessageBuffer._MessageSize = 0;
            #endif
        }

        Message::Message(uint8_t* Buffer, uint8_t size) {
            #if __has_include(<vector>)
                // If std::vector is available, we can use it directly
                _Data = std::vector<uint8_t>(Buffer, Buffer + size);  // Copy the buffer into the vector
            #else
                // If std::vector is not available, we allocate memory and copy the data manually
                _BufferSize = size;
                _MessageSize = size;

                // Allocate memory for the data
                _Data = (uint8_t*)calloc(size, sizeof(uint8_t));
                if (_Data == nullptr) {
                    // Handle allocation failure (return or throw exception, depending on your design)
                    return;
                }

                // Copy the data from the provided buffer
                for (uint8_t i = 0; i < size; ++i) {
                    _Data[i] = Buffer[i];
                }
            #endif
        }

        Message& Message::operator=(const Message& Source) {
            #if __has_include(<vector>)
                // If using std::vector, directly copy the data from Source
                _Data = Source._Data;
            #else
                // If not using std::vector, we need to handle raw memory
                if (this != &Source) {
                    // Resize the buffer if necessary
                    if (!_resize(Source._MessageSize)) {
                        return *this; // Handle failure by returning the current object
                    }

                    _BufferSize = Source._MessageSize;
                    _MessageSize = Source._MessageSize;

                    // Copy data byte by byte
                    for (size_t i = 0; i < _MessageSize; ++i) {
                        _Data[i] = Source._Data[i];
                    }
                }
            #endif

            return *this;
        }
    
        Message& Message::operator=(Message&& Source) noexcept {
            #if __has_include(<vector>)
                // If using std::vector, move the data from Source
                _Data = std::move(Source._Data);
            #else
                // If not using std::vector, handle raw memory
                if (this != &Source) {
                    // Free any existing data before moving
                    if (_Data != nullptr) {
                        free(_Data);
                    }

                    // Move the raw buffer and metadata
                    _Data = Source._Data;
                    _BufferSize = Source._BufferSize;
                    _MessageSize = Source._MessageSize;

                    // Nullify Source's data to prevent double-free
                    Source._Data = nullptr;
                    Source._BufferSize = 0;
                    Source._MessageSize = 0;
                }
            #endif

            return *this;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  std::vector
            
            #if __has_include(<vector>)
                // Copy constructor for std::vector
                Message::Message(const std::vector<uint8_t>& MessageBuffer) {
                    // Directly copy the data from the vector
                    _Data = MessageBuffer;
                }

                // Move constructor for std::vector
                Message::Message(std::vector<uint8_t>&& MessageBuffer) noexcept {
                    // Move the data from the vector
                    _Data = std::move(MessageBuffer);
                }
            
                // Copy assignment operator for std::vector
                Message& Message::operator=(const std::vector<uint8_t>& MessageBuffer) {
                    _Data = MessageBuffer;  // Simply copy the vector
                    return *this;
                }

                // Move assignment operator for std::vector
                Message& Message::operator=(std::vector<uint8_t>&& MessageBuffer) noexcept {
                    _Data = std::move(MessageBuffer);  // Move the vector
                    return *this;
                }
            #endif
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Data Access

        const uint8_t* Message::Buffer() const {
            #if __has_include(<vector>)
                return _Data.empty() ? nullptr : _Data.data();
            #else
                return _Data ? _Data : nullptr;
            #endif
        }

        const uint8_t Message::Data(size_t index){
            if(index < size()){
                return _Data[index];
            }
            return 0;
        }
        
        const size_t Message::size() const{
            
            #if __has_include(<vector>)
                return _Data.size();
            #else
                return _Size;
            #endif
        }
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Vector API

        #if __has_include(<vector>)

            // Getter for the internal buffer as a constant reference to std::vector
            const std::vector<uint8_t>& Message::Vector() const {
                return _Data;  // Return the internal vector
            }

            // Iterator to the beginning of the buffer (non-const)
            std::vector<uint8_t>::iterator Message::begin() noexcept {
                return _Data.begin();  // Return an iterator to the beginning
            }

            // Constant iterator to the beginning of the buffer
            std::vector<uint8_t>::const_iterator Message::begin() const noexcept {
                return _Data.begin();  // Return a constant iterator to the beginning
            }

            // Iterator to the end of the buffer (non-const)
            std::vector<uint8_t>::iterator Message::end() noexcept {
                return _Data.end();  // Return an iterator to the end
            }

            // Constant iterator to the end of the buffer
            std::vector<uint8_t>::const_iterator Message::end() const noexcept {
                return _Data.end();  // Return a constant iterator to the end
            }

        #endif // __has_include(<vector>)
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel Voice

        Message& Message::NoteOff(uint8_t Pitch, uint8_t Vel, uint8_t Channel) {
            // Resize the buffer to accommodate the message (3 bytes: command, pitch, velocity)
            if (_resize(3)) {
                // Set the Note Off command (0x80), with the specified channel
                _Data[0] = MIDI_NOTE_OFF + ((Channel<15)?Channel:15);  // 0x80 is the Note Off command, and Channel is masked to 4 bits
                _Data[1] = (Pitch<127)?Pitch:127;                    // Pitch value (which note to turn off)
                _Data[2] = (Vel<127)?Vel:127;                        // Velocity value (optional, can be 0)
            }

            return *this;
        }

        Message& Message::NoteOn(uint8_t Pitch, uint8_t Vel, uint8_t Channel) {
            // Resize the buffer to accommodate the message (3 bytes: command, pitch, velocity)
            if (_resize(3)) {
                // Set the Note On command (0x90), with the specified channel
                _Data[0] = MIDI_NOTE_ON + ((Channel<15)?Channel:15);  // 0x90 is the Note On command, and Channel is masked to 4 bits
                _Data[1] = (Pitch<127)?Pitch:127;                    // Pitch value (which note to turn off)
                _Data[2] = (Vel<127)?Vel:127;                        // Velocity value (optional, can be 0)
            }

            return *this;
        }

        Message& Message::AfterTouch(uint8_t Pitch, uint8_t Pressure, uint8_t Channel) {
            // Resize the buffer to accommodate the message (3 bytes: command, Pitch, pressure)
            if (_resize(3)) {
                _Data[0] = MIDI_AFTER_TOUCH + ((Channel<15)?Channel:15);  // AfterTouch command, and Channel is masked to 4 bits
                _Data[1] = (Pitch<127)?Pitch:127;                         // Pitch value
                _Data[2] = (Pressure<127)?Pressure:127;                   // Pressure Value
            }

            return *this;
        }

        Message& Message::ControlChange(uint8_t ControllerNumber, uint8_t Value, uint8_t Channel) {
            // Resize the buffer to accommodate the message (3 bytes: command, controller number, value)
            if (_resize(3)) {
                _Data[0] = MIDI_CONTROL_CHANGE + ((Channel<15)?Channel:15);  // AfterTouch command, and Channel is masked to 4 bits
                _Data[1] = (ControllerNumber<127)?ControllerNumber:127;   // Pitch value
                _Data[2] = (Value<127)?Value:127;                         // Pressure Value
            }

            return *this;
        }

        Message& Message::ProgramChange(uint8_t Program, uint8_t Channel) {

            // Resize the buffer to accommodate the message (2 bytes: command, program number)
            if (_resize(2)) {
                _Data[0] = MIDI_PROGRAM_CHANGE + ((Channel<15)?Channel:15);  // 0xC0 is the Program Change command, and Channel is masked to 4 bits
                _Data[1] = (Program<127)?Program:127;;                  // The program change number
            }

            return *this;
        }

        Message& Message::ChannelPressure(uint8_t Pressure, uint8_t Channel) {
            // Resize the buffer to accommodate the message (2 bytes: command, pressure value)
            if (_resize(2)) {
                // Clamp Channel to the valid range [0, 15]
                uint8_t clampedChannel = (Channel < 15) ? Channel : 15;

                // Clamp Pressure to the valid range [0, 127]
                uint8_t clampedPressure = (Pressure < 127) ? Pressure : 127;

                // Populate the buffer
                _Data[0] = MIDI_CHANNEL_PRESSURE + clampedChannel; // 0xD0 + channel
                _Data[1] = clampedPressure;                       // Pressure value
            }

            return *this;
        }

        Message& Message::PitchBend(int8_t Value, uint8_t Channel) {

            const uint16_t midpoint = 8192;  // 14-bit midpoint for no pitch bend
            uint16_t ValueScaled = (static_cast<uint16_t>(abs(Value)) << 6);  // Shift by 6 bits for 14-bit scaling

            // Apply sign to adjust around the midpoint
            uint16_t val = (Value >= 0) ? (midpoint + ValueScaled) : (midpoint - ValueScaled);

            if (_resize(3)) {
                _Data[0] = MIDI_PITCH_BEND + ((Channel<15)?Channel:15);  // Set the command and channel
                _Data[1] = (val & 0x7F);           // Extract the 7-bit LSB
                _Data[2] = ((val >> 7) & 0x7F);    // Extract the 7-bit MSB
            }

            return *this;
        }
        
        Message& Message::PitchBend(int16_t Value, uint8_t Channel) {
            const uint16_t midpoint = 8192;  // 14-bit midpoint for no pitch bend

            // Clamp the Value to the valid MIDI pitch bend range [-8192, 8191]
            if (Value < -8192) {
                Value = -8192;
            } else if (Value > 8191) {
                Value = 8191;
            }

            // Map the signed 14-bit value [-8192, 8191] to the unsigned 14-bit MIDI range [0, 16383]
            uint16_t val = static_cast<uint16_t>(Value + midpoint);

            if (_resize(3)) {
                // Set command and clamp the channel to the valid range [0, 15]
                _Data[0] = MIDI_PITCH_BEND + ((Channel < 16) ? Channel : 15);

                // Extract the 7-bit LSB and MSB from the 14-bit value
                _Data[1] = static_cast<uint8_t>(val & 0x7F);        // LSB
                _Data[2] = static_cast<uint8_t>((val >> 7) & 0x7F); // MSB
            }

            return *this;
        }

        Message& Message::PitchBend(uint8_t Value, uint8_t Channel) {
            // Resize the buffer to accommodate the message (3 bytes: command, lsb, msb)
            uint16_t ValueMapped = ((uint16_t)Value<<6);

            if (_resize(3)) {
                _Data[0] = MIDI_PITCH_BEND + ((Channel<15)?Channel:15);  // 0xE0 is the Pitch Bend command, and Channel is masked to 4 bits
                _Data[1] = (ValueMapped & 0x7F);           // 7-bit LSB (Least Significant Byte)
                _Data[2] = ((ValueMapped >> 7) & 0x7F);    // 7-bit MSB (Most Significant Byte)
            }

            return *this;
        }

        Message& Message::PitchBend(uint16_t Value, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, lsb, msb)
        if (Value > 0x3FFF) {
            Value = 0x3FFF;
        }

        if (_resize(3)) {
            _Data[0] = MIDI_PITCH_BEND + ((Channel<15)?Channel:15);  // 0xE0 is the Pitch Bend command, and Channel is masked to 4 bits
            _Data[1] = (Value & 0x7F);           // 7-bit LSB (Least Significant Byte)
            _Data[2] = ((Value >> 7) & 0x7F);    // 7-bit MSB (Most Significant Byte)
        }

        return *this;
    }

        #if defined(MIDILAR_MIDI_NOTE)
            Message& Message::NoteOff(const MidiNote& Source) {
                // Assuming that MidiNote has methods for pitch, velocity, and channel
                uint8_t pitch = Source.Pitch();      // Retrieve the pitch from the MidiNote
                uint8_t velocity = Source.Velocity();  // Retrieve the velocity from the MidiNote
                uint8_t channel = Source.Channel();   // Retrieve the channel from the MidiNote

                // Resize the buffer to accommodate the message (3 bytes)
                if (_resize(3)) {
                    _Data[0] = 0x80 + Channel;  // 0x80 is the Note Off command, and Channel is masked to 4 bits
                    _Data[1] = pitch;                    // Pitch value
                    _Data[2] = velocity;                  // Velocity value
                }

                return *this;
            }

            Message& Message::NoteOn(const MCC::MidiNote& Source) {
                // Assuming that MidiNote has methods for pitch, velocity, and channel
                uint8_t pitch = Source.Pitch();      // Retrieve the pitch from the MidiNote
                uint8_t velocity = Source.Velocity();  // Retrieve the velocity from the MidiNote
                uint8_t channel = Source.Channel();   // Retrieve the channel from the MidiNote

                // Resize the buffer to accommodate the message (3 bytes)
                if (_resize(3)) {
                    _Data[0] = 0x90 + Channel;  // 0x90 is the Note On command, and Channel is masked to 4 bits
                    _Data[1] = pitch;                    // Pitch value
                    _Data[2] = velocity;                  // Velocity value
                }

                return *this;
            }
        #endif

        #if defined(MCC_MUSICAL_NOTE_ENABLED)
            Message& Message::NoteOff(const MusicalNote::Note& Source, uint8_t Vel, uint8_t Channel) {
                uint8_t pitch = Source.Pitch();  // Retrieve pitch from MusicalNote::Note
                // Resize the buffer to accommodate the message (3 bytes)
                if (_resize(3)) {
                    _Data[0] = 0x80 + Channel;  // 0x80 is the Note Off command, and Channel is masked to 4 bits
                    _Data[1] = pitch;                    // Pitch value
                    _Data[2] = Vel;                      // Velocity value
                }

                return *this;
            }

            Message& Message::NoteOn(const MusicalNote::Note& Source, uint8_t Vel, uint8_t Channel) {
                uint8_t pitch = Source.Pitch();  // Retrieve pitch from MusicalNote::Note
                // Resize the buffer to accommodate the message (3 bytes)
                if (_resize(3)) {
                    _Data[0] = 0x90 + Channel;  // 0x90 is the Note On command, and Channel is masked to 4 bits
                    _Data[1] = pitch;                    // Pitch value
                    _Data[2] = Vel;                      // Velocity value
                }

                return *this;
            }

            Message& Message::NoteOff(const MusicalNote::Pitch& Source, uint8_t Octave, uint8_t Vel, uint8_t Channel) {
                uint8_t pitch = Source.Pitch() + (Octave * 12);  // Assuming that Pitch() returns the pitch in semitones and Octave adjusts it
                // Resize the buffer to accommodate the message (3 bytes)
                if (_resize(3)) {
                    _Data[0] = 0x80 + Channel;  // 0x80 is the Note Off command, and Channel is masked to 4 bits
                    _Data[1] = pitch;                    // Pitch value
                    _Data[2] = Vel;                      // Velocity value
                }

                return *this;
            }

            Message& Message::NoteOn(const MusicalNote::Pitch& Source, uint8_t Octave, uint8_t Vel, uint8_t Channel) {
                uint8_t pitch = Source.Pitch() + (Octave * 12);  // Assuming that Pitch() returns the pitch in semitones and Octave adjusts it
                // Resize the buffer to accommodate the message (3 bytes)
                if (_resize(3)) {
                    _Data[0] = 0x90 + Channel;  // 0x90 is the Note On command, and Channel is masked to 4 bits
                    _Data[1] = pitch;                    // Pitch value
                    _Data[2] = Vel;                      // Velocity value
                }

                return *this;
            }
        #endif
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel Mode Messages
    
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Emergency Messages

            Message& Message::SetAllSoundOff(uint8_t Channel) {
                // Clamp Channel
                if (Channel > 15) { Channel = 15; }
                // Resize the buffer to accommodate the message (2 bytes: command, controller number)
                if (_resize(3)) {
                    _Data[0] = MIDI_CONTROL_CHANGE + Channel;     // All Sound Off is a Control Change command
                    _Data[1] = MIDI_CM_ALL_SOUND_OFF;      // Controller number for All Sound Off
                    _Data[2] = 0x00;                    // Value not used
                }

                return *this;
            }

            Message& Message::SetAllNotesOff(uint8_t Channel) {
                // Clamp Channel
                if (Channel > 15) { Channel = 15; }
                // Resize the buffer to accommodate the message (2 bytes: command, controller number)
                if (_resize(3)) {
                    _Data[0] = MIDI_CONTROL_CHANGE + Channel;        // All Notes Off is a Control Change command
                    _Data[1] = MIDI_CM_ALL_NOTES_OFF;        // Controller number for All Notes Off
                    _Data[2] = 0x00;        // Value to turn off all notes
                }

                return *this;
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Local Control

            Message& Message::SetLocalControl(bool mode, uint8_t channel) {
                return SetLocalControl(static_cast<uint8_t>(mode ? MIDI_CM_LOCAL_CONTROL_ON : MIDI_CM_LOCAL_CONTROL_OFF), channel);
            }

            Message& Message::SetLocalControl(const MidiProtocol::LocalControlStatus& mode, uint8_t channel) {
                return SetLocalControl(static_cast<uint8_t>(mode), channel);
            }

            Message& Message::SetLocalControl(uint8_t mode, uint8_t channel) {
                if (_resize(3)) {
                    uint8_t clampedChannel = (channel > 15) ? 15 : channel;
                    _Data[0] = MIDI_CONTROL_CHANGE + clampedChannel; // Command + channel
                    _Data[1] = MIDI_CM_LOCAL_CONTROL;                  // Control Change: Local Control
                    _Data[2] = mode;                                // Mode (On/Off)
                }
                return *this;
            }

            Message& Message::SetLocalControlOn(uint8_t Channel) {
                return SetLocalControl((uint8_t)MIDI_CM_LOCAL_CONTROL_ON, Channel);  // 0x7F is the value for "On" in MIDI Local Control
            }

            Message& Message::SetLocalControlOff(uint8_t Channel) {
                return SetLocalControl((uint8_t)MIDI_CM_LOCAL_CONTROL_OFF, Channel);  // 0x00 is the value for "Off" in MIDI Local Control
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Omni

            Message& Message::SetOmniMode(uint8_t Mode, uint8_t Channel) {
                // Clamp Channel
                if (Channel > 15) { Channel = 15; }
                // Resize the buffer to accommodate the message (3 bytes: command, controller number, omni mode value)
                if(Mode == MIDI_CM_OMNI_OFF || Mode == MIDI_CM_OMNI_ON){
                    if (_resize(3)) {
                        _Data[0] = MIDI_CONTROL_CHANGE + Channel; // Channel Mode Change is a Control Change command
                        _Data[1] = Mode;                // Controller number for Omni Mode (0x7C)
                        _Data[2] = 0;                   // Unused
                    }
                } else {
                    _resize(0);
                }
                

                return *this;
            }
            
            Message& Message::SetOmniMode(const MidiProtocol::ChannelMode& Mode, uint8_t Channel) {
                if(Mode == MidiProtocol::ChannelMode::OmniOn || Mode == MidiProtocol::ChannelMode::OmniOff){
                    SetOmniMode(static_cast<uint8_t>(Mode), Channel);
                }
                else{
                    _resize(0);
                }
                return *this;
            }
            
            Message& Message::SetOmniMode(bool Mode, uint8_t Channel) {
                return SetOmniMode(static_cast<uint8_t>(Mode ? MIDI_CM_OMNI_ON : MIDI_CM_OMNI_OFF), Channel);
            }

            Message& Message::SetOmniOn(uint8_t Channel) {
                return SetOmniMode((uint8_t)MIDI_CM_OMNI_ON, Channel);  // 0x00 is the value for "Off" in MIDI Local Control
            }

            Message& Message::SetOmniOff(uint8_t Channel) {
                return SetOmniMode((uint8_t)MIDI_CM_OMNI_OFF, Channel);  // 0x00 is the value for "Off" in MIDI Local Control
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Polyphony

            Message& Message::SetPolyphony(uint8_t Mode, uint8_t Channel) {
                // Clamp Channel
                if (Channel > 15) { Channel = 15; }
                // Resize the buffer to accommodate the message (3 bytes: command, controller number, polyphony mode value)
                if(Mode == MIDI_CM_POLY_ON || Mode == MIDI_CM_MONO_ON){
                    if (_resize(3)) {
                        _Data[0] = MIDI_CONTROL_CHANGE + Channel; // Channel Mode Change is a Control Change command
                        _Data[1] = Mode;                // Controller number for Omni Mode (0x7C)
                        _Data[2] = 0;                   // Unused
                    }
                } else {
                    _resize(0);
                }

                return *this;
            }
            
            Message& Message::SetPolyphony(bool Mode, uint8_t Channel) {
                return SetPolyphony(static_cast<uint8_t>(Mode ? MIDI_CM_POLY_ON : MIDI_CM_MONO_ON), Channel);
            }

            Message& Message::SetPolyphony(const MidiProtocol::ChannelMode& Mode, uint8_t Channel) {
                if(Mode == MidiProtocol::ChannelMode::Mono || Mode == MidiProtocol::ChannelMode::Poly){
                    SetPolyphony(static_cast<uint8_t>(Mode), Channel);
                }
                else{
                    _resize(0);
                }
                return *this;
            }

            Message& Message::SetMono(uint8_t Channel) {
                return SetPolyphony((uint8_t)MIDI_CM_MONO_ON, Channel);  // 0x00 sets Mono mode
            }

            Message& Message::SetPoly(uint8_t Channel) {
            return SetPolyphony((uint8_t)MIDI_CM_POLY_ON, Channel);  // 0x7F sets Poly mode
        }     
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // System Common Messages

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // MIDI Time Code

            Message& Message::MTC_QuarterFrame(uint8_t TimeComponent, uint8_t Data) {
                if (_resize(2)) {
                    _Data[0] = MIDI_MTC_QUARTER_FRAME;
                    _Data[1] = ((TimeComponent & 0x07) << 4) | (Data & 0x0F);
                }
                return *this;
            }

            Message& Message::MTC_QuarterFrame(const MidiProtocol_MTC::TimeComponent& TimeComponent, uint8_t Data) {
                return MTC_QuarterFrame(static_cast<uint8_t>(TimeComponent), Data);
            }


            Message& Message::MTC_FullFrame(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Frames, uint8_t Framerate, uint8_t SysexChannel) {
                if (_resize(8)) {
                    _Data[0] = MIDI_SYSEX_START;                 // Start of SysEx
                    _Data[2] = MIDI_SYSEX_RT_MTC_FULL_FRAME;  // Full Frame identifier
                    _Data[1] = SysexChannel;         // SysEx Channel
                    _Data[3] = ((Framerate & 0x03) << 5) | (Hours & 0x1F); // Framerate + Hours
                    _Data[4] = Minutes & 0x3F;       // Minutes
                    _Data[5] = Seconds & 0x3F;       // Seconds
                    _Data[6] = Frames & 0x1F;        // Frames
                    _Data[7] = MIDI_SYSEX_END;                 // End of SysEx
                }
                return *this;
            }

            Message& Message::MTC_FullFrame(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Frames, const MidiProtocol_MTC::FrameRate& Framerate, uint8_t SysexChannel) {
                return MTC_FullFrame(Hours, Minutes, Seconds, Frames, static_cast<uint8_t>(Framerate), SysexChannel);
            }        
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Constructs a Song Position Pointer message
            Message& Message::SongPositionPointer(uint16_t Position) {
            if (_resize(3)) {
                _Data[0] = MIDI_SONG_POSITION_POINTER; // Song Position Pointer status byte
                _Data[1] = Position & 0x7F;           // Least significant 7 bits
                _Data[2] = (Position >> 7) & 0x7F;    // Most significant 7 bits
            }
            return *this;
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Constructs a Song Select message
            Message& Message::SongSelect(uint8_t Song) {
            if (_resize(2)) {
                _Data[0] = MIDI_SONG_SELECT; // Song Select status byte
                _Data[1] = Song & 0x7F;      // Ensure only the lower 7 bits are used
            }
            return *this;
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Constructs a Tuning Request message
            Message& Message::TuningRequest() {
            if (_resize(1)) {
                _Data[0] = MIDI_TUNING_REQUEST; // Tuning Request status byte
            }
            return *this;
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }