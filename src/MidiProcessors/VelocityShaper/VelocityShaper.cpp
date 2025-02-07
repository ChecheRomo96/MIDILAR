#include "VelocityShaper.h"

namespace MIDILAR::MidiProcessors {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Constructor for VelocityShaper.
     * Initializes the MIDI message parser and sets up default LUT parameters.
     */
        VelocityShaper::VelocityShaper()
            : _InputChannels(0xFFFF) { // Default: process all channels

            // Bind parser callbacks
            _MessageParser.BindChannelVoiceCallback(
                this,                                         // Pass `this` as the instance
                &VelocityShaper::StaticChannelVoiceCallback  // Pass static function pointer
            );

            _MessageParser.BindDefaultCallback(
                this,  // Pass `this` as the instance
                &VelocityShaper::StaticDefaultCallback  // Pass static function pointer
            );

            // Configure LUT
            _LUT.SetInputRange(0,127);
            _LUT.SetBufferSize(128);
            _LUT.SetExponentiationGain(5.0);
            _LUT.SetAmplitude(127);
            _LUT.SetOffset(0);
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Processes incoming MIDI messages.
     * @param Message Pointer to the MIDI message buffer.
     * @param Size The size of the incoming message.
     */
        void VelocityShaper::MidiInput(const uint8_t* Message, size_t Size) override{
            _MessageParser.ProcessData(Message, Size);
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Callback for MIDI Channel Voice Messages (e.g., Note-On, Note-Off).
     * @param Data MIDI message.
     */
        void VelocityShaper::_ChannelVoiceCallback(const MidiFoundation::Message& Data) {
            // Only process Note-On and Note-Off
            if (Data.Type != MidiFoundation::MessageType::NoteOn &&
                Data.Type != MidiFoundation::MessageType::NoteOff) {
                _MidiOutput(Data);
                return;
            }

            auto messageIn = Data.Buffer();

            uint8_t channel = messageIn[0] & 0x0F;
            uint8_t velocity = messageIn[2];

            if(velocity == 0){
                _MidiOutput(Data);
                return;
            }

            if( !((_InputChannels >> channel) & 0b1) ){
                _MidiOutput(Data);
                return;
            }

            // Apply LogExpLUT transformation
            uint8_t shapedVelocity = _LUT.GetValue(velocity);

            // Note-On message with velocity 0 is interpreted as Note-Off in MIDI
            if (shapedVelocity == 0 && Data.Type == MidiFoundation::MessageType::NoteOn) {
                shapedVelocity = 1;
            }


            uint8_t messageOut[3];
            messageOut[0] = messageIn[0];
            messageOut[1] = messageIn[1];
            messageOut[2] = shapedVelocity;

            // Forward transformed message
            MidiFoundation::Message modifiedMsg;
            modifiedMsg.SetRawData(messageOut,3);

            _MidiOutput(modifiedMsg);  // Send processed message
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Default callback for unprocessed MIDI messages.
     * @param Data MIDI message.
     */
        void VelocityShaper::_DefaultCallback(const MidiFoundation::Message& Data) {
            _MidiOutput(Data);  // Forward unprocessed messages
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////

        void VelocityShaper::SetMorph(float setMorph) {
            if (_LUT.Morph() != setMorph) {  // Avoid redundant updates
                _LUT.SetMorph(setMorph);
            }
        }

        float VelocityShaper::Morph() const {
            return _LUT.Morph();  // Fetch current morph value from LUT
        }

        void VelocityShaper::SetExpGain(float expGain) {
            if (expGain > 0.0f && _LUT.ExponentiationGain() != expGain) {  // Ensure valid exponentiation gain
                _LUT.SetExponentiationGain(expGain);
            }
        }

        float VelocityShaper::ExpGain() const {
            return _LUT.ExponentiationGain();  // Fetch current exponentiation gain from LUT
        }

    //
    ///////////////////////////////////////////////////////////////////////////////////////////////

        

        void VelocityShaper::StaticChannelVoiceCallback(void* context, const uint8_t* Data, size_t Size) {
            if (context) {
                static_cast<VelocityShaper*>(context)->_ChannelVoiceCallback(Data, Size);
            }
        }

        void VelocityShaper::StaticDefaultCallback(void* context, const uint8_t* Data, size_t Size) {
            if (context) {
                static_cast<VelocityShaper*>(context)->_DefaultCallback(Data, Size);
            }
        }

} // namespace MIDILAR::MidiProcessors
