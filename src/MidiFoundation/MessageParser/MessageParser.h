#ifndef MIDILAR_MIDI_MESSAGE_PARSER_H
#define MIDILAR_MIDI_MESSAGE_PARSER_H

    #include <System/BuildSettings.h>
	#include <SystemFoundation/CallbackHandler/CallbackHandler.h>
	#include <MidiFoundation/Message/Message.h>

	namespace MIDILAR::MidiFoundation{


		class MessageParser {
	
		public:
		
		    using CallbackType = MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t>::CallbackType;

		private:

		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _channelVoiceCallback;
			bool InvokeChannelVoiceCallback(const uint8_t* Data, size_t Size);

		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _controlChangeCallback;
			bool InvokeControlChangeCallback(const uint8_t* Data, size_t Size);

		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _realTimeCallback;
			bool InvokeRealTimeCallback(const uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _systemCommonCallback;
			bool InvokeSystemCommonCallback(const uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _sysExCallback;
			bool InvokeSysExCallback(const uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _mtcCallback;
			bool InvokeMTCCallback(const uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _mscCallback;
			bool InvokeMSCCallback(const uint8_t* Data, size_t Size);
			
		    MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _defaultCallback;
			bool InvokeDefaultCallback(const uint8_t* Data, size_t Size);
			


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
		    void BindChannelVoiceCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindChannelVoiceCallback(T* instance) {
				_channelVoiceCallback.bind<T, Method>(instance);
			}

		    void BindControlChangeCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindControlChangeCallback(T* instance) {
				_controlChangeCallback.bind<T, Method>(instance);
			}

		    void BindRealTimeCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindRealTimeCallback(T* instance) {
				_realTimeCallback.bind<T, Method>(instance);
			}

		    void BindSystemCommonCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindSystemCommonCallback(T* instance) {
				_systemCommonCallback.bind<T, Method>(instance);
			}

		    void BindSysExCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindSysExCallback(T* instance) {
				_sysExCallback.bind<T, Method>(instance);
			}

		    void BindMTCCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindMTCCallback(T* instance) {
				_mtcCallback.bind<T, Method>(instance);
			}

		    void BindMSCCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindMSCCallback(T* instance) {
				_mscCallback.bind<T, Method>(instance);
			}

		    void BindDefaultCallback(CallbackType callback);
			
			template <typename T, void (T::*Method)(const uint8_t*, size_t)>
			inline void BindDefaultCallback(T* instance) {
				_defaultCallback.bind<T, Method>(instance);
			}

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