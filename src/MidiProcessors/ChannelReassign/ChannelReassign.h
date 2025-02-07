#ifndef MIDILAR_CHANNEL_REASSIGN_PROCESSOR_H
#define MIDILAR_CHANNEL_REASSIGN_PROCESSOR_H

	#include <MidiFoundation/Processor/Processor.h>
	#include <MidiFoundation/MessageParser/MessageParser.h>

	namespace MIDILAR::MidiProcessors{

		class ChannelReassign : public MIDILAR::MidiFoundation::Processor{
		protected:
			MIDILAR::MidiFoundation::MessageParser _MessageParser;
			uint16_t _InputChannels;
			uint16_t _OutputChannels;

		private:
			void _ChannelVoiceCallback(const MidiFoundation::Message& Data);
			void _DefaultCallback(const MidiFoundation::Message& Data);

		public:

			ChannelReassign();

    		void MidiInput(const uint8_t* Message, size_t Size) override;
    		
    		static void StaticChannelVoiceCallback(void* context, const MidiFoundation::Message& msg);
    		static void StaticDefaultCallback(void* context, const MidiFoundation::Message& msg);

		};

	}

#endif//MIDILAR_CHANNEL_REASSIGN_PROCESSOR_H