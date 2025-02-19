#include "ChannelReassign.h"

namespace MIDILAR::MidiProcessors{

	ChannelReassign::ChannelReassign()
		: _MessageParser(3)
	{
	        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
	                        static_cast<uint32_t>(Capabilities::MidiOut));

	        _MessageParser.BindChannelVoiceCallback(
			    this,  										  // Pass `this` as the instance
			    &ChannelReassign::StaticChannelVoiceCallback  // Pass static function pointer
			);

	        _MessageParser.BindDefaultCallback(
			    this,  // Pass `this` as the instance
			    &ChannelReassign::StaticDefaultCallback  // Pass static function pointer
			);

	        _InputChannels = 0xFFFF;	// Listen all channels
	        _OutputChannels = 0x0001;	// Output on Channel 1

	}

	void ChannelReassign::_ChannelVoiceCallback(const uint8_t* Data, size_t Size) {
	    if (Size == 3) {  // Only process 3-byte Channel Voice Messages
	        uint8_t channel = Data[0] & 0x0F;  // Extract the MIDI channel number

	        // Check if the input channel is allowed
	        if ((_InputChannels >> channel) & 0b1) {
	            for (uint8_t i = 0; i < 16; i++) {
	                if ((_OutputChannels >> i) & 0b1) {
	                    uint8_t Message[3] = {
	                        static_cast<uint8_t>((Data[0] & 0xF0) | i),  // Preserve message type & set new channel
	                        Data[1],  // Note Number / Control Number
	                        Data[2]   // Velocity / Value
	                    };

	                    MidiOutput(Message, 3);  // Send the transformed message
	                }
	            }
	        }
	        else{
		        // Forward any messages that are for other channel directly
		        MidiOutput(Data, Size);
	        }
	    } 
	    else {
	        // Forward any non-channel voice messages directly
	        MidiOutput(Data, Size);
	    }
	}

	void ChannelReassign::_DefaultCallback(const uint8_t* Data, size_t Size){
		MidiOutput(Data, Size);
	}

	void ChannelReassign::MidiInput(const uint8_t* Message, size_t Size) {
		_MessageParser.ProcessData(Message,Size);
	}

	void ChannelReassign::SetInputChannels(uint16_t ChannelMap){
		_InputChannels = ChannelMap;
	}

	void ChannelReassign::SetOutputChannels(uint16_t ChannelMap){
		_OutputChannels = ChannelMap;
	}


	void ChannelReassign::StaticChannelVoiceCallback(void* context, const uint8_t* Data, size_t Size) {
        if (context) {
            static_cast<ChannelReassign*>(context)->_ChannelVoiceCallback(Data, Size);
        }
	}

	void ChannelReassign::StaticDefaultCallback(void* context, const uint8_t* Data, size_t Size) {
        if (context) {
            static_cast<ChannelReassign*>(context)->_DefaultCallback(Data, Size);
        }
	}
}