#include "Message.h"
#include "Defines.h"

namespace MIDILAR::MidiFoundation{

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

    // Constructors
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

    Message::Message(char* Buffer, uint8_t size) {
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

    // Constructor for when std::vector is available
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
    #endif

    // Copy assignment operator
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

    // Move assignment operator
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

    #if __has_include(<vector>)
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

    // Data Access

        const uint8_t* Message::Buffer() const{
            #if __has_include(<vector>)
                return &_Data[0];
            #else
                return _Data;
            #endif
        }

        const size_t Message::size() const{
            
            #if __has_include(<vector>)
                return _Data.size();
            #else
                return _Size;
            #endif
        }

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

    // Channel Voice

    Message& Message::NoteOff(uint8_t Pitch, uint8_t Vel, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, pitch, velocity)
        if (_resize(3)) {
            // Set the Note Off command (0x80), with the specified channel
            _Data[0] = MIDI_NOTE_OFF | (Channel & 0x0F);  // 0x80 is the Note Off command, and Channel is masked to 4 bits
            _Data[1] = Pitch;                    // Pitch value (which note to turn off)
            _Data[2] = Vel;                      // Velocity value (optional, can be 0)
        }

        return *this;
    }

    Message& Message::NoteOn(uint8_t Pitch, uint8_t Vel, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, pitch, velocity)
        if (_resize(3)) {
            // Set the Note On command (0x90), with the specified channel
            _Data[0] = MIDI_NOTE_ON | (Channel & 0x0F);  // 0x90 is the Note On command, and Channel is masked to 4 bits
            _Data[1] = Pitch;                    // Pitch value (which note to turn on)
            _Data[2] = Vel;                      // Velocity value (how loud the note should be)
        }

        return *this;
    }

    Message& Message::AfterTouch(uint8_t Note, uint8_t Pressure, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, note, pressure)
        if (_resize(3)) {
            _Data[0] = MIDI_AFTER_TOUCH | (Channel & 0x0F);  // 0xA0 is the AfterTouch command, and Channel is masked to 4 bits
            _Data[1] = Note;                     // The note that has aftertouch applied
            _Data[2] = Pressure;                 // The pressure value
        }

        return *this;
    }

    Message& Message::ControlChange(uint8_t ControllerNumber, uint8_t Value, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, controller number, value)
        if (_resize(3)) {
            _Data[0] = MIDI_CONTROL_CHANGE | (Channel & 0x0F);  // 0xB0 is the Control Change command, and Channel is masked to 4 bits
            _Data[1] = ControllerNumber;         // The controller number
            _Data[2] = Value;                    // The value for the controller
        }

        return *this;
    }

    Message& Message::ProgramChange(uint8_t Program, uint8_t Channel) {
        // Resize the buffer to accommodate the message (2 bytes: command, program number)
        if (_resize(2)) {
            _Data[0] = MIDI_PROGRAM_CHANGE | (Channel & 0x0F);  // 0xC0 is the Program Change command, and Channel is masked to 4 bits
            _Data[1] = Program;                  // The program change number
        }

        return *this;
    }

    Message& Message::ChannelPressure(uint8_t Pressure, uint8_t Channel) {
        // Resize the buffer to accommodate the message (2 bytes: command, pressure value)
        if (_resize(2)) {
            _Data[0] = MIDI_CHANNEL_PRESSURE | (Channel & 0x0F);  // 0xD0 is the Channel Pressure command, and Channel is masked to 4 bits
            _Data[1] = Pressure;                 // The channel pressure value
        }

        return *this;
    }

    Message& Message::PitchBend(int8_t Value, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, lsb, msb)
        if (_resize(3)) {
            _Data[0] = MIDI_PITCH_BEND | (Channel & 0x0F);  // 0xE0 is the Pitch Bend command, and Channel is masked to 4 bits
            _Data[1] = (Value & 0x7F);           // 7-bit LSB (Least Significant Byte)
            _Data[2] = ((Value >> 7) & 0x7F);    // 7-bit MSB (Most Significant Byte)
        }

        return *this;
    }

    Message& Message::PitchBend(int16_t Value, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, lsb, msb)
        if (_resize(3)) {
            _Data[0] = MIDI_PITCH_BEND | (Channel & 0x0F);  // 0xE0 is the Pitch Bend command, and Channel is masked to 4 bits
            _Data[1] = (Value & 0x7F);           // 7-bit LSB (Least Significant Byte)
            _Data[2] = ((Value >> 7) & 0x7F);    // 7-bit MSB (Most Significant Byte)
        }

        return *this;
    }

    Message& Message::PitchBend(uint8_t Value, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, lsb, msb)
        if (_resize(3)) {
            _Data[0] = MIDI_PITCH_BEND | (Channel & 0x0F);  // 0xE0 is the Pitch Bend command, and Channel is masked to 4 bits
            _Data[1] = Value;                    // 7-bit LSB (Least Significant Byte)
            _Data[2] = 0;                         // MSB is 0 for 8-bit pitch bend
        }

        return *this;
    }

    Message& Message::PitchBend(uint16_t Value, uint8_t Channel) {
        // Resize the buffer to accommodate the message (3 bytes: command, lsb, msb)
        if (_resize(3)) {
            _Data[0] = MIDI_PITCH_BEND | (Channel & 0x0F);  // 0xE0 is the Pitch Bend command, and Channel is masked to 4 bits
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
                _Data[0] = 0x80 | (channel & 0x0F);  // 0x80 is the Note Off command, and Channel is masked to 4 bits
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
                _Data[0] = 0x90 | (channel & 0x0F);  // 0x90 is the Note On command, and Channel is masked to 4 bits
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
                _Data[0] = 0x80 | (Channel & 0x0F);  // 0x80 is the Note Off command, and Channel is masked to 4 bits
                _Data[1] = pitch;                    // Pitch value
                _Data[2] = Vel;                      // Velocity value
            }

            return *this;
        }

        Message& Message::NoteOn(const MusicalNote::Note& Source, uint8_t Vel, uint8_t Channel) {
            uint8_t pitch = Source.Pitch();  // Retrieve pitch from MusicalNote::Note
            // Resize the buffer to accommodate the message (3 bytes)
            if (_resize(3)) {
                _Data[0] = 0x90 | (Channel & 0x0F);  // 0x90 is the Note On command, and Channel is masked to 4 bits
                _Data[1] = pitch;                    // Pitch value
                _Data[2] = Vel;                      // Velocity value
            }

            return *this;
        }

        Message& Message::NoteOff(const MusicalNote::Pitch& Source, uint8_t Octave, uint8_t Vel, uint8_t Channel) {
            uint8_t pitch = Source.Pitch() + (Octave * 12);  // Assuming that Pitch() returns the pitch in semitones and Octave adjusts it
            // Resize the buffer to accommodate the message (3 bytes)
            if (_resize(3)) {
                _Data[0] = 0x80 | (Channel & 0x0F);  // 0x80 is the Note Off command, and Channel is masked to 4 bits
                _Data[1] = pitch;                    // Pitch value
                _Data[2] = Vel;                      // Velocity value
            }

            return *this;
        }

        Message& Message::NoteOn(const MusicalNote::Pitch& Source, uint8_t Octave, uint8_t Vel, uint8_t Channel) {
            uint8_t pitch = Source.Pitch() + (Octave * 12);  // Assuming that Pitch() returns the pitch in semitones and Octave adjusts it
            // Resize the buffer to accommodate the message (3 bytes)
            if (_resize(3)) {
                _Data[0] = 0x90 | (Channel & 0x0F);  // 0x90 is the Note On command, and Channel is masked to 4 bits
                _Data[1] = pitch;                    // Pitch value
                _Data[2] = Vel;                      // Velocity value
            }

            return *this;
        }
    #endif

    // Channel Mode Messages

    Message& Message::SetLocalControl(uint8_t Mode) {
        // Resize the buffer to accommodate the message (2 bytes: command, mode)
        if (_resize(3)) {
            _Data[0] = MIDI_CONTROL_CHANGE;        // Local Control is a Control Change command
            _Data[1] = MIDI_LOCAL_CONTROL;        // Controller number for Local Control
            _Data[2] = Mode;        // 0x00 = Off, 0x7F = On
        }

        return *this;
    }

    Message& Message::SetLocalControlOn() {
        return SetLocalControl(MIDI_LOCAL_CONTROL_ON);  // 0x7F is the value for "On" in MIDI Local Control
    }

    Message& Message::SetLocalControlOff() {
        return SetLocalControl(MIDI_LOCAL_CONTROL_OFF);  // 0x00 is the value for "Off" in MIDI Local Control
    }

    Message& Message::SetAllSoundOff() {
        // Resize the buffer to accommodate the message (2 bytes: command, controller number)
        if (_resize(3)) {
            _Data[0] = MIDI_CONTROL_CHANGE;     // All Sound Off is a Control Change command
            _Data[1] = MIDI_ALL_SOUND_OFF;      // Controller number for All Sound Off
            _Data[2] = 0x00;                    // Value not used
        }

        return *this;
    }

    Message& Message::SetAllNotesOff() {
        // Resize the buffer to accommodate the message (2 bytes: command, controller number)
        if (_resize(3)) {
            _Data[0] = MIDI_CONTROL_CHANGE;        // All Notes Off is a Control Change command
            _Data[1] = MIDI_ALL_NOTES_OFF;        // Controller number for All Notes Off
            _Data[2] = 0x00;        // Value to turn off all notes
        }

        return *this;
    }

    Message& Message::SetChannelMode(uint8_t Omni, uint8_t Poly) {
        // Resize the buffer to accommodate the message (3 bytes: command, omni value, poly value)
        SetChannelMode_Omni(Omni);
        SetChannelMode_Polyphony(Poly);

        return *this;
    }

    Message& Message::SetChannelMode_Omni(uint8_t Mode) {
        // Resize the buffer to accommodate the message (3 bytes: command, controller number, omni mode value)
        if(Mode == MIDI_OMNI_MODE_OFF || Mode == MIDI_OMNI_MODE_ON){
            if (_resize(3)) {
                _Data[0] = MIDI_CONTROL_CHANGE; // Channel Mode Change is a Control Change command
                _Data[1] = Mode;                // Controller number for Omni Mode (0x7C)
                _Data[2] = 0;                   // Unused
            }
        } else {
            if (_resize(1)) {
                _Data[0] = 0; // Empty message
            }
        }
        

        return *this;
    }

    Message& Message::SetChannelMode_OmniOn() {
        return SetChannelMode_Omni(MIDI_OMNI_MODE_ON);  // 0x01 enables Omni mode
    }

    Message& Message::SetChannelMode_OmniOff() {
        return SetChannelMode_Omni(MIDI_OMNI_MODE_OFF);  // 0x00 disables Omni mode
    }

    Message& Message::SetChannelMode_Polyphony(uint8_t Mode) {
        // Resize the buffer to accommodate the message (3 bytes: command, controller number, polyphony mode value)
         if(Mode == MIDI_POLY_MODE_ON || Mode == MIDI_MONO_MODE_ON){
            if (_resize(3)) {
                _Data[0] = MIDI_CONTROL_CHANGE; // Channel Mode Change is a Control Change command
                _Data[1] = Mode;                // Controller number for Omni Mode (0x7C)
                _Data[2] = 0;                   // Unused
            }
        } else {
            if (_resize(1)) {
                _Data[0] = 0; // Empty message
            }
        }

        return *this;
    }

    Message& Message::SetChannelMode_Mono() {
        return SetChannelMode_Polyphony(MIDI_MONO_MODE_ON);  // 0x00 sets Mono mode
    }

    Message& Message::SetChannelMode_Poly() {
        return SetChannelMode_Polyphony(MIDI_POLY_MODE_ON);  // 0x7F sets Poly mode
    }
}