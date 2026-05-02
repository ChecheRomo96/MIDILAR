#include "ChannelReassign.h"

namespace MIDILAR::MidiDevices{

	ChannelReassign::ChannelReassign()
        : MIDILAR::MidiCore::DeviceBase()
		, _MessageParser(3)
	{
	        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
	                        static_cast<uint32_t>(Capabilities::MidiOut));

	        _MessageParser.BindChannelVoiceCallback<ChannelReassign, &ChannelReassign::_ChannelVoiceCallback>(this);
	        _MessageParser.BindDefaultCallback<ChannelReassign, &ChannelReassign::_DefaultCallback>(this);

	        _InputChannels = 0xFFFF;	// Listen all channels
	        _OutputChannels = 0x0001;	// Output on Channel 1

	}

	void ChannelReassign::_ChannelVoiceCallback(const uint8_t* Data, size_t Size) {
	}

	void ChannelReassign::_DefaultCallback(const uint8_t* Data, size_t Size){
		MidiOutput(Data, Size);
	}

	void ChannelReassign::MidiInput(const uint8_t* MessageIn, size_t Size) {
		
	    if (Size == 3) {  // Only process 3-byte Channel Voice Messages
	        uint8_t channel = MessageIn[0] & 0x0F;  // Extract the MIDI channel number

	        // Check if the input channel is allowed
	        if ((_InputChannels >> channel) & 0b1) {
	            for (uint8_t i = 0; i < 16; i++) {
	                if ((_OutputChannels >> i) & 0b1) {
	                    uint8_t MessageOut[3] = {
	                        static_cast<uint8_t>((MessageIn[0] & 0xF0) | i),  // Preserve message type & set new channel
	                        MessageIn[1],  // Note Number / Control Number
	                        MessageIn[2]   // Velocity / Value
	                    };

	                    MidiOutput(MessageOut, 3);  // Send the transformed message
	                }
	            }
	        }
	        else{
		        // Forward any messages that are for other channel directly
		        MidiOutput(MessageIn, Size);
	        }
	    } 
	    else {
	        // Forward any non-channel voice messages directly
	        MidiOutput(MessageIn, Size);
	    }
	}

	void ChannelReassign::SetInputChannels(uint16_t ChannelMap){
		_InputChannels = ChannelMap;
	}

	void ChannelReassign::SetOutputChannels(uint16_t ChannelMap){
		_OutputChannels = ChannelMap;
	}
}