#ifndef MIDILAR_CHANNEL_REASSIGN_DEVICE_H
#define MIDILAR_CHANNEL_REASSIGN_DEVICE_H

    #include <MIDILAR_BuildSettings.h>
	#include <MidiCore/DeviceBase/DeviceBase.h>
	#include <MidiCore/MessageParser/MessageParser.h>

	namespace MIDILAR::MidiDevices{

		class ChannelReassign : public MIDILAR::MidiCore::DeviceBase {
		protected:
			MIDILAR::MidiCore::MessageParser _MessageParser;
			uint16_t _InputChannels;
			uint16_t _OutputChannels;
			
			void _ChannelVoiceCallback(const uint8_t* Message, size_t Size);
			void _DefaultCallback(const uint8_t* Message, size_t Size);

		public:

			ChannelReassign();

    		void MidiInput(const uint8_t* Message, size_t Size) override;

			void SetInputChannels(uint16_t ChannelMap);
			void SetOutputChannels(uint16_t ChannelMap);
		};

	}

#endif//MIDILAR_CHANNEL_REASSIGN_DEVICE_H