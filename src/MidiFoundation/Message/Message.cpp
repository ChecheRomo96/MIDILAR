#include "Message.h"

namespace MIDILAR::MidiFoundation{

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private resize method

        bool Message::_resize(size_t new_size) {
            #if __has_include(<vector>)
                if (new_size <= _Data.size()) {
                    _Data.resize(new_size);
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
                    _MessageSize = new_size;
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
                _MessageSize = new_size;
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
                return (_Data && _MessageSize > 0) ? _Data : nullptr;
            #endif
        }

        uint8_t Message::Data(size_t index) const {
            if (index < size()) {
                #if __has_include(<vector>)
                    return _Data[index];
                #else
                    return (_Data ? _Data[index] : 0);
                #endif
            }
            return 0; // Out-of-bounds access returns 0
        }
        
        size_t Message::size() const{
            
            #if __has_include(<vector>)
                return _Data.size();
            #else
                return _MessageSize;
            #endif
        }

        Message& Message::SetRawData(const uint8_t* Data, size_t Size) {
            if (!Data || Size == 0) {
                _resize(0);
                return *this;
            }

            #if __has_include(<vector>)
                if (Data == _Data.data()){ return *this; } // Avoid unnecessary self-assignment
                _Data.assign(Data, Data + Size);
            #else
                if (Data == _Data) return; // Avoid self-assignment
                if (Size > _BufferSize) {
                    if (!_resize(Size)) {
                        _resize(0);
                        return *this;
                    }
                }
                memcpy(_Data, Data, Size);
                _MessageSize = Size;
            #endif

            return *this;
        }


        #if __has_include(<vector>)
            Message& Message::SetRawData(const std::vector<uint8_t>& Data) {
                if (Data.empty()) {
                    _resize(0); // Ensure consistency when clearing data
                    return *this;
                }
                return SetRawData(Data.data(), Data.size());
            }
        #endif  
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
            std::vector<uint8_t>::const_iterator Message::cbegin() const noexcept {
                return _Data.cbegin();  // Return a constant iterator to the beginning
            }

            // Iterator to the end of the buffer (non-const)
            std::vector<uint8_t>::iterator Message::end() noexcept {
                return _Data.end();  // Return an iterator to the end
            }

            // Constant iterator to the end of the buffer
            std::vector<uint8_t>::const_iterator Message::cend() const noexcept {
                return _Data.cend();  // Return a constant iterator to the end
            }

        #endif // __has_include(<vector>)
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel Voice

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Note Messages

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
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // After Touch

            Message& Message::AfterTouch(uint8_t Pitch, uint8_t Pressure, uint8_t Channel) {
                // Resize the buffer to accommodate the message (3 bytes: command, Pitch, pressure)
                if (_resize(3)) {
                    _Data[0] = MIDI_AFTER_TOUCH + ((Channel<15)?Channel:15);  // AfterTouch command, and Channel is masked to 4 bits
                    _Data[1] = (Pitch<127)?Pitch:127;                         // Pitch value
                    _Data[2] = (Pressure<127)?Pressure:127;                   // Pressure Value
                }

                return *this;
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Control Change

            Message& Message::ControlChange(uint8_t ControllerNumber, uint8_t Value, uint8_t Channel) {
                // Resize the buffer to accommodate the message (3 bytes: command, controller number, value)
                if (_resize(3)) {
                    _Data[0] = MIDI_CONTROL_CHANGE + ((Channel<15)?Channel:15);  // AfterTouch command, and Channel is masked to 4 bits
                    _Data[1] = (ControllerNumber<127)?ControllerNumber:127;   // Pitch value
                    _Data[2] = (Value<127)?Value:127;                         // Pressure Value
                }

                return *this;
            }

            /////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Sub Groups

                /////////////////////////////////////////////////////////////////////////////////////////////////////
                // General CC

                    Message& Message::CC_BankSelect(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_BANK_SELECT, Value, Channel);
                    }

                    Message& Message::CC_Modulation(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_MODULATION, Value, Channel);
                    }

                    Message& Message::CC_BreathControl(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_BREATH_CONTROL, Value, Channel);
                    }

                    Message& Message::CC_FootPedal(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_FOOT_PEDAL, Value, Channel);
                    }

                    Message& Message::CC_Portamento(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_PORTAMENTO, Value, Channel);
                    }

                    Message& Message::CC_Volume(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_VOLUME, Value, Channel);
                    }

                    Message& Message::CC_Balance(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_BALANCE, Value, Channel);
                    }

                    Message& Message::CC_Pan(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_PAN, Value, Channel);
                    }

                    Message& Message::CC_Expression(uint8_t Value, uint8_t Channel){
                        return ControlChange(MIDI_EXPRESSION, Value, Channel);
                    }
                //
                /////////////////////////////////////////////////////////////////////////////////////////////////////
                // Channel Mode

                    /////////////////////////////////////////////////////////////////////////////////////////////////
                    // SetAllSoundOff
                    
                        Message& Message::CC_AllSoundOff(uint8_t Channel){
                            return ControlChange(MIDI_ALL_SOUND_OFF, 0x00, Channel);
                        }
                    //
                    /////////////////////////////////////////////////////////////////////////////////////////////////
                    // SetAllNotesOff

                        Message& Message::CC_AllNotesOff(uint8_t Channel){
                            return ControlChange(MIDI_ALL_NOTES_OFF, 0x00, Channel);
                        }
                    //
                    /////////////////////////////////////////////////////////////////////////////////////////////////
                    // Local Control
                        
                        Message& Message::CC_LocalControl(bool Mode, uint8_t Channel){
                            return CC_LocalControl(static_cast<uint8_t>(Mode ? MIDI_LOCAL_CONTROL_ON : MIDI_LOCAL_CONTROL_OFF), Channel);
                        }
                        
                        Message& Message::CC_LocalControl(uint8_t Mode, uint8_t Channel){
                            return ControlChange(MIDI_LOCAL_CONTROL, Mode, Channel);
                        }

                        Message& Message::CC_LocalControl(const MidiProtocol::LocalControlStatus& Mode, uint8_t Channel){
                            return CC_LocalControl(static_cast<uint8_t>(Mode), Channel);
                        }

                        Message& Message::CC_LocalControlOn(uint8_t Channel){
                            return CC_LocalControl((uint8_t)MIDI_LOCAL_CONTROL_ON, Channel);
                        }

                        Message& Message::CC_LocalControlOff(uint8_t Channel){
                            return CC_LocalControl((uint8_t)MIDI_LOCAL_CONTROL_OFF, Channel);
                        }
                    //
                    /////////////////////////////////////////////////////////////////////////////////////////////////
                    // Omni
                    
                        Message& Message::CC_OmniMode(uint8_t Mode, uint8_t Channel) {
                            // Resize the buffer to accommodate the message (3 bytes: command, controller number, omni mode value)
                            if(Mode == MIDI_OMNI_OFF || Mode == MIDI_OMNI_ON){
                                return ControlChange(Mode, 0, Channel);
                            } else {
                                _resize(0);
                            }
                            return *this;
                        }
                        
                        Message& Message::CC_OmniMode(const MidiProtocol::ChannelMode& Mode, uint8_t Channel) {
                            if(Mode == MidiProtocol::ChannelMode::OmniOn || Mode == MidiProtocol::ChannelMode::OmniOff){
                                return CC_OmniMode(static_cast<uint8_t>(Mode), Channel);
                            }
                            else{
                                _resize(0);
                            }
                            return *this;
                        }
                        
                        Message& Message::CC_OmniMode(bool Mode, uint8_t Channel) {
                            return CC_OmniMode(static_cast<uint8_t>(Mode ? MIDI_OMNI_ON : MIDI_OMNI_OFF), Channel);
                        }

                        Message& Message::CC_OmniOn(uint8_t Channel) {
                            return CC_OmniMode((uint8_t)MIDI_OMNI_ON, Channel);  // 0x00 is the value for "Off" in MIDI Local Control
                        }

                        Message& Message::CC_OmniOff(uint8_t Channel) {
                            return CC_OmniMode((uint8_t)MIDI_OMNI_OFF, Channel);  // 0x00 is the value for "Off" in MIDI Local Control
                        }
                    //
                    /////////////////////////////////////////////////////////////////////////////////////////////////
                    // Polyphony

                        Message& Message::CC_Polyphony(uint8_t Mode, uint8_t Channel) {
                            // Resize the buffer to accommodate the message (3 bytes: command, controller number, polyphony mode value)
                            if(Mode == MIDI_POLY_ON || Mode == MIDI_MONO_ON){
                                return ControlChange(Mode, 0, Channel);
                            } else {
                                _resize(0);
                            }

                            return *this;
                        }
                        
                        Message& Message::CC_Polyphony(bool Mode, uint8_t Channel) {
                            return CC_Polyphony(static_cast<uint8_t>(Mode ? MIDI_POLY_ON : MIDI_MONO_ON), Channel);
                        }

                        Message& Message::CC_Polyphony(const MidiProtocol::ChannelMode& Mode, uint8_t Channel) {
                            if(Mode == MidiProtocol::ChannelMode::Mono || Mode == MidiProtocol::ChannelMode::Poly){
                                return CC_Polyphony(static_cast<uint8_t>(Mode), Channel);
                            }
                            else{
                                _resize(0);
                            }
                            return *this;
                        }

                        Message& Message::CC_Mono(uint8_t Channel) {
                            return CC_Polyphony((uint8_t)MIDI_MONO_ON, Channel);  // 0x00 sets Mono mode
                        }

                        Message& Message::CC_Poly(uint8_t Channel) {
                            return CC_Polyphony((uint8_t)MIDI_POLY_ON, Channel);  // 0x7F sets Poly mode
                        }
                    //
                    /////////////////////////////////////////////////////////////////////////////////////////////////
                //
                /////////////////////////////////////////////////////////////////////////////////////////////////////
                // RPN & NRPN Messages

                    Message& Message::CC_NRPN(uint16_t ParameterID, uint8_t Data){

                        return *this;
                    }

                    Message& Message::CC_NRPN(uint16_t ParameterID, uint16_t Data){

                        return *this;
                    }

                    Message& Message::CC_NRPN_DataIncrement(){

                        return *this;
                    }

                    Message& Message::CC_NRPN_DataDecrement(){

                        return *this;
                    }
                //
                /////////////////////////////////////////////////////////////////////////////////////////////////////
            //
            /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Program Change

            Message& Message::ProgramChange(uint8_t Program, uint8_t Channel) {

                // Resize the buffer to accommodate the message (2 bytes: command, program number)
                if (_resize(2)) {
                    _Data[0] = MIDI_PROGRAM_CHANGE + ((Channel<15)?Channel:15);  // 0xC0 is the Program Change command, and Channel is masked to 4 bits
                    _Data[1] = (Program<127)?Program:127;;                  // The program change number
                }

                return *this;
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Channel Pressure

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
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pitch Bend
            
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
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Real-Time Messages
        
        Message& Message::TimingTick(){
            if (_resize(1)) {
                _Data[0] = MIDI_REALTIME_TIMING_TICK;
            }
            return *this;
        }

        Message& Message::Start(){
            if (_resize(1)) {
                _Data[0] = MIDI_REALTIME_START;
            }
            return *this;
        }

        Message& Message::Continue(){
            if (_resize(1)) {
                _Data[0] = MIDI_REALTIME_CONTINUE;
            }
            return *this;
        }

        Message& Message::Stop(){
            if (_resize(1)) {
                _Data[0] = MIDI_REALTIME_STOP;
            }
            return *this;
        }

        Message& Message::ActiveSensing(){
            if (_resize(1)) {
                _Data[0] = MIDI_REALTIME_ACTIVE_SENSING;
            }
            return *this;
        }

        Message& Message::SystemReset(){
            if (_resize(1)) {
                _Data[0] = MIDI_REALTIME_SYSTEM_RESET;
            }
            return *this;
        }
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

        Message& Message::SystemExclusive(uint8_t* Data, uint8_t Length) {
            // Validate input
            if (Length == 0) {
                return *this;  // No data to process
            }

            // Calculate actual size (including 0xF0 and 0xF7 if missing)
            size_t finalSize = Length;
            if (Data[0] != 0xF0) finalSize++;  // Add space for 0xF0 if missing
            if (Data[Length - 1] != 0xF7) finalSize++;  // Add space for 0xF7 if missing

            // **Resize buffer before writing data**
            if (!_resize(finalSize)) {
                return *this;  // Memory allocation failed, return unchanged message
            }

            size_t index = 0;

            // **Add SysEx Start (0xF0) if missing**
            if (Data[0] != 0xF0) {
                _Data[index++] = 0xF0;
            }


            // **Copy SysEx Data (Avoid Duplicating Start/End Bytes)**
            for (size_t i = 0; i < Length; i++) {
                _Data[index++] = Data[i];
            }
            
            // **Add SysEx End (0xF7) if missing**
            if (Data[Length - 1] != 0xF7) {
                _Data[index++] = 0xF7;
            }

            return *this;
        }

    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }