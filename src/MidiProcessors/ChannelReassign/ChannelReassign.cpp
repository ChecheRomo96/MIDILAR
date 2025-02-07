#include "ChannelReassign.h"

namespace MIDILAR::MidiProcessors{

	ChannelReassign::ChannelReassign()
		: _MessageParser(3)
	{
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
                        static_cast<uint32_t>(Capabilities::MidiOut));

        _MessageParser.BindChannelVoiceCallback([this](const MidiFoundation::Message& Data) {
		    this->_ChannelVoiceCallback(Data);
		});
		
        _MessageParser.BindDefaultCallback([this](const MidiFoundation::Message& Data) {
		    this->_DefaultCallback(Data);
		});

        _InputChannels = 0xFFFF;	// Listen all channels
        _OutputChannels = 0x0001;	// Output on Channel 1

	}

	void ChannelReassign::_ChannelVoiceCallback(const MidiFoundation::Message& Data) {
	    if (Data.size() == 3) {  // Only process 3-byte Channel Voice Messages
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

	                    _MidiOutput(Message, 3);  // Send the transformed message
	                }
	            }
	        }
	        else{
		        // Forward any messages that are for other channel directly
		        _MidiOutput(Data);
	        }
	    } 
	    else {
	        // Forward any non-channel voice messages directly
	        _MidiOutput(Data);
	    }
	}

	void ChannelReassign::_DefaultCallback(const MidiFoundation::Message& Data){
		_MidiOutput(Data);
	}

	void MidiInput(const uint8_t* Message, size_t Size) override {
		_MessageParser.ProcessData(Message,Size);
	}
}