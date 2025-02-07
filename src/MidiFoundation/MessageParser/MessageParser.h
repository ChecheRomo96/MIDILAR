#ifndef MIDILAR_MIDI_MESSAGE_PARSER_H
#define MIDILAR_MIDI_MESSAGE_PARSER_H

	#include <MIDILAR_BuildSettings.h>
	#include <SystemFoundation/CallbackHandler/CallbackHandler.h>
	#include <MidiFoundation/Message/Message.h>

	namespace MIDILAR::MidiFoundation{


		class MessageParser {
	
		public:
		
		    using CallbackType = MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&>::CallbackType;
		    using CallbackReferenceType = MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&>::CallbackReferenceType;

		private:
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _channelVoiceCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _controlChangeCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _realTimeCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _systemCommonCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _sysExCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _mtcCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _mscCallback;
		    MIDILAR::SystemFoundation::CallbackHandler<void, const MidiFoundation::Message&> _defaultCallback;


			enum class Status : uint8_t{
				Idle = 0,
				Processing = 1,
				ProcessingSysex = 2
			};

		    MidiFoundation::Message _message;
			Status _Status;
			size_t _MessageSize;
			uint8_t* _MessageBuffer;
			size_t _MessageBufferSize;

		public:
		    // **Constructor**
		    MessageParser();

		    MessageParser(size_t BufferSize);

			~MessageParser();

			bool ResizeBuffer(size_t NewSize);

		    // **Set Callbacks Using `CallbackHandler`**
		    void BindChannelVoiceCallback(CallbackReferenceType callback);
		    void BindControlChangeCallback(CallbackReferenceType callback);
		    void BindRealTimeCallback(CallbackReferenceType callback);
		    void BindSystemCommonCallback(CallbackReferenceType callback);
		    void BindSysExCallback(CallbackReferenceType callback);
		    void BindMTCCallback(CallbackReferenceType callback);
		    void BindMSCCallback(CallbackReferenceType callback);
		    void BindDefaultCallback(CallbackReferenceType callback);

		    // **Unbind Methods**
		    void UnbindChannelVoiceCallback();
		    void UnbindControlChangeCallback();
		    void UnbindRealTimeCallback();
		    void UnbindSystemCommonCallback();
		    void UnbindSysExCallback();
		    void UnbindMTCCallback();
		    void UnbindMSCCallback();
		    void UnbindDefaultCallback();

		    // **Unbind All Callbacks**
		    void UnbindAll();

		    // **Process Raw MIDI Data**
		    void ProcessData(const uint8_t* data, size_t size);

		private:

			void _StatusByteHandler(uint8_t data);
			void _RealTimeHandler();
			void _ChannelVoiceHandler();
			void _SystemCommonHandler();
			void _SysExHandler();

		};
	}

#endif//MIDILAR_MIDI_MESSAGE_PARSER_H