#ifndef MIDILAR_CHANNEL_REASSIGN_PROCESSOR_H
#define MIDILAR_CHANNEL_REASSIGN_PROCESSOR_H

    #include <System/BuildSettings.h>
	#include <MidiFoundation/Processor/Processor.h>
	#include <MidiFoundation/MessageParser/MessageParser.h>

	namespace MIDILAR::MidiProcessors{

		class ChannelReassign : public MIDILAR::MidiFoundation::Processor{
		protected:
			MIDILAR::MidiFoundation::MessageParser _MessageParser;
			uint16_t _InputChannels;
			uint16_t _OutputChannels;

		private:
			void _ChannelVoiceCallback(const uint8_t* Data, size_t Size);
			void _DefaultCallback(const uint8_t* Data, size_t Size);

		public:



			ChannelReassign();

    		void MidiInput(const uint8_t* Message, size_t Size) override;

			void SetInputChannels(uint16_t ChannelMap);
			void SetOutputChannels(uint16_t ChannelMap);
    		
    		static void StaticChannelVoiceCallback(void* context, const uint8_t* Data, size_t Size);
    		static void StaticDefaultCallback(void* context, const uint8_t* Data, size_t Size);

		};

	}

#endif//MIDILAR_CHANNEL_REASSIGN_PROCESSOR_H