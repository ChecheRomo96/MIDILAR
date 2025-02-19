#include "VelocityShaper.h"

namespace MIDILAR::MidiProcessors {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Constructor for VelocityShaper.
     * Initializes the MIDI message parser and sets up default LUT parameters.
     */
        VelocityShaper::VelocityShaper()
            : MIDILAR::MidiFoundation::Processor(), _InputChannels(0xFFFF) { // Default: process all channels

            // Bind parser callbacks
            _MessageParser.BindChannelVoiceCallback(
                this,                                         // Pass `this` as the instance
                &VelocityShaper::StaticChannelVoiceCallback  // Pass static function pointer
            );

            _MessageParser.BindDefaultCallback(
                this,  // Pass `this` as the instance
                &VelocityShaper::StaticDefaultCallback  // Pass static function pointer
            );

            _capabilities = static_cast<uint32_t>(MIDILAR::MidiFoundation::Processor::Capabilities::MidiIn | MIDILAR::MidiFoundation::Processor::Capabilities::MidiOut);

            // Configure LUT
            //_LUT.SetInputRange(0,127);
            //_LUT.SetBufferSize(128);
            //_LUT.SetExponentiationGain(5.0);
            //_LUT.SetAmplitude(127);
            //_LUT.SetOffset(0);
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Processes incoming MIDI messages.
     * @param Message Pointer to the MIDI message buffer.
     * @param Size The size of the incoming message.
     */
        void VelocityShaper::MidiInput(const uint8_t* Message, size_t Size){
            _MessageParser.ProcessData(Message, Size);
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Callback for MIDI Channel Voice Messages (e.g., Note-On, Note-Off).
     * @param Data MIDI message.
     */
        void VelocityShaper::_ChannelVoiceCallback(const uint8_t* Message, size_t Size) {

            if(Size != 3){ MidiOutput(Message,Size); return; }
            // Only process Note-On and Note-Off
            if (Message[0]&0xF0 != MIDI_NOTE_ON && Message[0]&0xF0 != MIDI_NOTE_OFF) {
                MidiOutput(Message,Size);
                return;
            }


            uint8_t channel = Message[0] & 0x0F;
            uint8_t velocity = Message[2];

            if(velocity == 0 || velocity == 127){
                MidiOutput(Message,Size);
                return;
            }

            if( !((_InputChannels >> channel) & 0b1) ){
                MidiOutput(Message,Size);
                return;
            }

            // Apply LogExpLUT transformation
            uint8_t shapedVelocity = 1; //_LUT.GetValue(velocity);

            // Note-On message with velocity 0 is interpreted as Note-Off in MIDI
            if (shapedVelocity == 0 && Message[0]&0xF0 == MIDI_NOTE_ON) {
                shapedVelocity = 1;
            }


            uint8_t messageOut[3];
            messageOut[0] = Message[0];
            messageOut[1] = Message[1];
            messageOut[2] = shapedVelocity;

            MidiOutput(messageOut, 3);  // Send processed message
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Default callback for unprocessed MIDI messages.
     * @param Data MIDI message.
     */
        void VelocityShaper::_DefaultCallback(const uint8_t* Message, size_t Size) {
            MidiOutput(Message,Size);  // Forward unprocessed messages
        }
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////

    
		void VelocityShaper::SetInputChannels(uint16_t ChannelMap){
            _InputChannels = ChannelMap;
        }

        void VelocityShaper::SetMorph(float setMorph) {
            if (true){//_LUT.GetMorph() != setMorph) {  // Avoid redundant updates
                //_LUT.SetMorph(setMorph);
            }
        }

        float VelocityShaper::Morph() const {
            return 0.0;//_LUT.GetMorph();  // Fetch current morph value from LUT
        }

        void VelocityShaper::SetExpGain(float expGain) {
            if (expGain > 0.0f && true){//_LUT.GetExponentiationGain() != expGain) {  // Ensure valid exponentiation gain
                //_LUT.SetExponentiationGain(expGain);
            }
        }

        float VelocityShaper::ExpGain() const {
            return 1.0;//_LUT.GetExponentiationGain();  // Fetch current exponentiation gain from LUT
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
