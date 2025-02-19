#ifndef MIDILAR_MIDI_MESSAGE_PARSER_H
#define MIDILAR_MIDI_MESSAGE_PARSER_H

    #include <System/BuildSettings.h>
	#include <SystemFoundation/CallbackHandler/CallbackHandler.h>
	#include <MidiFoundation/Message/Message.h>

	namespace MIDILAR::MidiFoundation{


		class MessageParser {
	
		public:
		
		    using CallbackType = MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t>::CallbackType;
		    using CallbackReferenceType = MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t>::CallbackReferenceType;

		private:
			struct CallbackContext {
			    void* instance;  // Pointer to the instance (could be any class)
			    void (*invoke)(void*, const uint8_t*, size_t);  // Function pointer for invocation
			};

		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _channelVoiceCallback;
			CallbackContext _channelVoiceContext;
			bool InvokeChannelVoiceCallback(uint8_t* Data, size_t Size);

		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _controlChangeCallback;
			CallbackContext _controlChangeContext;
			bool InvokeControlChangeCallback(uint8_t* Data, size_t Size);

		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _realTimeCallback;
			CallbackContext _realTimeContext;
			bool InvokeRealTimeCallback(uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _systemCommonCallback;
			CallbackContext _systemCommonContext;
			bool InvokeSystemCommonCallback(uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _sysExCallback;
			CallbackContext _sysExContext;
			bool InvokeSysExCallback(uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _mtcCallback;
			CallbackContext _mtcContext;
			bool InvokeMTCCallback(uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _mscCallback;
			CallbackContext _mscContext;
			bool InvokeMSCCallback(uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _defaultCallback;
			CallbackContext _defaultContext;
			bool InvokeDefaultCallback(uint8_t* Data, size_t Size);
			


			enum class Status : uint8_t{
				Idle = 0,
				Processing = 1,
				ProcessingSysex = 2
			};

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
		    void BindChannelVoiceCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindControlChangeCallback(CallbackReferenceType callback);
		    void BindControlChangeCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindRealTimeCallback(CallbackReferenceType callback);
		    void BindRealTimeCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindSystemCommonCallback(CallbackReferenceType callback);
		    void BindSystemCommonCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindSysExCallback(CallbackReferenceType callback);
		    void BindSysExCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindMTCCallback(CallbackReferenceType callback);
		    void BindMTCCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindMSCCallback(CallbackReferenceType callback);
		    void BindMSCCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));

		    void BindDefaultCallback(CallbackReferenceType callback);
		    void BindDefaultCallback(void* instance, void (*invokeFunc)(void*, const uint8_t*, size_t));


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