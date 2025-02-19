#include "MessageParser.h"

namespace MIDILAR::MidiFoundation {

    // **Constructor**
        MessageParser::MessageParser()
            : MessageParser(3)
        {
        }

        MessageParser::MessageParser(size_t BufferSize)
            : _Status(Status::Idle),
              _MessageBuffer(nullptr),
              _MessageBufferSize(0),
              _MessageSize(0),
              _channelVoiceContext({nullptr, nullptr}),
              _controlChangeContext({nullptr, nullptr}),
              _realTimeContext({nullptr, nullptr}),
              _systemCommonContext({nullptr, nullptr}),
              _sysExContext({nullptr, nullptr}),
              _mtcContext({nullptr, nullptr}),
              _mscContext({nullptr, nullptr}),
              _defaultContext({nullptr, nullptr})
        {
        	ResizeBuffer(BufferSize);
        }

    // **Destructor**
        MessageParser::~MessageParser() {
            if (_MessageBuffer) {
                free(_MessageBuffer);
                _MessageBuffer = nullptr;
            }
        }

    // **Resize Message Buffer**
        bool MessageParser::ResizeBuffer(size_t NewSize) {
    	    uint8_t* new_data = (uint8_t*)realloc(_MessageBuffer, NewSize * sizeof(uint8_t));
    	    if (new_data == nullptr) {
    	        return false; // Memory allocation failed
    	    }

    	    _MessageBuffer = new_data;
    	    _MessageBufferSize = NewSize;

    	    // Ensure that _MessageSize does not exceed buffer capacity
    	    if (_MessageSize > NewSize) {
    	        _MessageSize = NewSize;
    	    }

    	    return true;
        }

    // **Set Callback Handlers**
        void MessageParser::BindChannelVoiceCallback( CallbackReferenceType callback ){
            _channelVoiceCallback.bind(callback);
            _channelVoiceContext = {nullptr, nullptr};
        }

        void MessageParser::BindChannelVoiceCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _channelVoiceCallback.unbind();
            _channelVoiceContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindChannelVoiceCallback() {
            _channelVoiceCallback.unbind();
            _channelVoiceContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeChannelVoiceCallback(uint8_t* msg, size_t size) {
            
            if (_channelVoiceContext.instance && _channelVoiceContext.invoke) {
                _channelVoiceContext.invoke(_channelVoiceContext.instance, msg, size);
                return true;
            } 
            
            if (_channelVoiceCallback.status()) {
                _channelVoiceCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindControlChangeCallback( CallbackReferenceType callback ){
            _controlChangeCallback.bind(callback);
            _controlChangeContext = {nullptr, nullptr};
        }

        void MessageParser::BindControlChangeCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _controlChangeCallback.unbind();
            _controlChangeContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindControlChangeCallback() {
            _controlChangeCallback.unbind();
            _controlChangeContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeControlChangeCallback(uint8_t* msg, size_t size) {
            
            if (_controlChangeContext.instance && _controlChangeContext.invoke) {
                _controlChangeContext.invoke(_controlChangeContext.instance, msg, size);
                return true;
            } 
            
            if (_controlChangeCallback.status()) {
                _controlChangeCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindRealTimeCallback( CallbackReferenceType callback ){
            _realTimeCallback.bind(callback);
            _realTimeContext = {nullptr, nullptr};
        }

        void MessageParser::BindRealTimeCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _realTimeCallback.unbind();
            _realTimeContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindRealTimeCallback() {
            _realTimeCallback.unbind();
            _realTimeContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeRealTimeCallback(uint8_t* msg, size_t size) {
            
            if (_realTimeContext.instance && _realTimeContext.invoke) {
                _realTimeContext.invoke(_realTimeContext.instance, msg, size);
                return true;
            } 
            
            if (_realTimeCallback.status()) {
                _realTimeCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindSystemCommonCallback( CallbackReferenceType callback ){
            _systemCommonCallback.bind(callback);
            _systemCommonContext = {nullptr, nullptr};
        }

        void MessageParser::BindSystemCommonCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _systemCommonCallback.unbind();
            _systemCommonContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindSystemCommonCallback() {
            _systemCommonCallback.unbind();
            _systemCommonContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeSystemCommonCallback(uint8_t* msg, size_t size) {
            
            if (_systemCommonContext.instance && _systemCommonContext.invoke) {
                _systemCommonContext.invoke(_systemCommonContext.instance, msg, size);
                return true;
            } 
            
            if (_systemCommonCallback.status()) {
                _systemCommonCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindSysExCallback( CallbackReferenceType callback ){
            _sysExCallback.bind(callback);
            _sysExContext = {nullptr, nullptr};
        }

        void MessageParser::BindSysExCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _sysExCallback.unbind();
            _sysExContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindSysExCallback() {
            _sysExCallback.unbind();
            _sysExContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeSysExCallback(uint8_t* msg, size_t size) {
            
            if (_sysExContext.instance && _sysExContext.invoke) {
                _sysExContext.invoke(_sysExContext.instance, msg, size);
                return true;
            } 
            
            if (_sysExCallback.status()) {
                _sysExCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindMTCCallback( CallbackReferenceType callback ){
            _mtcCallback.bind(callback);
            _mtcContext = {nullptr, nullptr};
        }

        void MessageParser::BindMTCCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _mtcCallback.unbind();
            _mtcContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindMTCCallback() {
            _mtcCallback.unbind();
            _mtcContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeMTCCallback(uint8_t* msg, size_t size) {
            
            if (_mtcContext.instance && _mtcContext.invoke) {
                _mtcContext.invoke(_mtcContext.instance, msg, size);
                return true;
            } 
            
            if (_mtcCallback.status()) {
                _mtcCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindMSCCallback( CallbackReferenceType callback ){
            _mscCallback.bind(callback);
            _mscContext = {nullptr, nullptr};
        }

        void MessageParser::BindMSCCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _mscCallback.unbind();
            _mscContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindMSCCallback() {
            _mscCallback.unbind();
            _mscContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeMSCCallback(uint8_t* msg, size_t size) {
            
            if (_mscContext.instance && _mscContext.invoke) {
                _mscContext.invoke(_mscContext.instance, msg, size);
                return true;
            } 
            
            if (_mscCallback.status()) {
                _mscCallback.invoke(msg, size);
                return true;
            }

            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindDefaultCallback( CallbackReferenceType callback ){
            _defaultCallback.bind(callback);
            _defaultContext = {nullptr, nullptr};
        }

        void MessageParser::BindDefaultCallback( void* instance,  void (*invokeFunc)(void*, const uint8_t*, size_t) ){
            _defaultCallback.unbind();
            _defaultContext = {instance, invokeFunc};
        }

        void MessageParser::UnbindDefaultCallback() {
            _defaultCallback.unbind();
            _defaultContext = {nullptr, nullptr}; 
        }

        bool MessageParser::InvokeDefaultCallback(uint8_t* msg, size_t size) {
            
            if (_defaultContext.instance && _defaultContext.invoke) {
                _defaultContext.invoke(_defaultContext.instance, msg, size);
                return true;
            } 
            
            if (_defaultCallback.status()) {
                _defaultCallback.invoke(msg, size);
                return true;
            }

            return false;
        }


    // **Unbind All Callbacks at Once**
        void MessageParser::UnbindAll() {
            _channelVoiceCallback.unbind();
            _controlChangeCallback.unbind();
            _realTimeCallback.unbind();
            _systemCommonCallback.unbind();
            _sysExCallback.unbind();
            _mtcCallback.unbind();
            _mscCallback.unbind();
            _defaultCallback.unbind();
        }

    // **Process Incoming MIDI Data**
        void MessageParser::ProcessData(const uint8_t* data, size_t size) {
            if (size == 0) return;

            for (size_t i = 0; i < size; i++) {
                _StatusByteHandler(data[i]);

                // Append New Data To Buffer
                if (_Status == Status::Processing || _Status == Status::ProcessingSysex) {
                    if (_MessageSize < _MessageBufferSize) {
                        _MessageBuffer[_MessageSize++] = data[i];
                    } else {
                        // Buffer Overflow
                        _Status = Status::Idle;
                    }
                }

                // Check Message
                if (_Status == Status::Processing) {
                    _RealTimeHandler();
                    _ChannelVoiceHandler();
                    _SystemCommonHandler();
                }

                // SysEx Message Handling
                if (_Status == Status::ProcessingSysex) {
                    _SysExHandler();
                }
            }
        }

    // **Status Byte Handling**
        void MessageParser::_StatusByteHandler(uint8_t data) {
            if (data & 0x80) { // **Status Byte Detected**
                if (_Status == Status::ProcessingSysex) {
                    if(data != MIDI_SYSEX_END){
                        // **Unexpected Status Byte Inside SysEx → Discard Message**
                        _Status = Status::Idle;
                        _MessageSize = 0;
                    }
                    
                    return;
                }

                if (data == MIDI_SYSEX_START) { 
                    _Status = Status::ProcessingSysex;
                    _MessageSize = 0;
                } else {
                    _Status = Status::Processing;
                    _MessageSize = 0;
                }
            }
        }

    // **Real-Time Message Handling**
        void MessageParser::_RealTimeHandler() {
            if (_MessageSize == 1) {
                switch (_MessageBuffer[0]) {
                    case MIDI_REALTIME_TIMING_TICK:
                    case MIDI_REALTIME_START:
                    case MIDI_REALTIME_CONTINUE:
                    case MIDI_REALTIME_STOP:
                    case MIDI_REALTIME_ACTIVE_SENSING:
                    case MIDI_REALTIME_SYSTEM_RESET:
                        if (!InvokeRealTimeCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        } 
                        _Status = Status::Idle;
                        _MessageSize = 0;
                        break;
                }
            }
        }

    // **Channel Voice & Control Change Messages**
        void MessageParser::_ChannelVoiceHandler() {
        if (_MessageSize == 2) {
            switch (_MessageBuffer[0] & 0xF0) {
                case MIDI_CHANNEL_PRESSURE:
                case MIDI_PROGRAM_CHANGE:
                    if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default:
                    break;
            }
        }

        if (_MessageSize == 3) { 
            switch (_MessageBuffer[0] & 0xF0) {
                case MIDI_CONTROL_CHANGE:
                    if (!InvokeControlChangeCallback(_MessageBuffer, _MessageSize)) {
                        if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        }
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                case MIDI_NOTE_OFF:
                case MIDI_NOTE_ON:
                case MIDI_AFTER_TOUCH:
                case MIDI_PITCH_BEND:
                    if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default:
                    break;
            }
        }
    }


    // **System Common Messages**
        void MessageParser::_SystemCommonHandler() {
            if (_MessageSize == 1) {
                switch (_MessageBuffer[0]) {
                    case MIDI_TUNING_REQUEST:
                        if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        }
                        _Status = Status::Idle;
                        _MessageSize = 0;
                        break;
                }
            } 
            else if (_MessageSize == 2) {
                switch (_MessageBuffer[0]) {
                    case MIDI_MTC_QUARTER_FRAME:

                        if (!InvokeMTCCallback(_MessageBuffer, _MessageSize)) { 
                            if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                                InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                            }
                        }

                        _Status = Status::Idle;
                        _MessageSize = 0;
                        break;

                    case MIDI_SONG_SELECT:
                        
                        if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        }

                        _Status = Status::Idle;
                        _MessageSize = 0;
                        break;

                    default: break;
                }
            }
            else if (_MessageSize == 3) {
                switch (_MessageBuffer[0]) {
                    case MIDI_SONG_POSITION_POINTER:

                        if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        }

                        _Status = Status::Idle;
                        _MessageSize = 0;
                        break;

                    default: break;
                }
            }
        }

    // **SysEx Message Handling**
        void MessageParser::_SysExHandler() {
            if (_MessageSize > 0 && _MessageBuffer[_MessageSize - 1] == MIDI_SYSEX_END) { // SysEx message complete

                if (!InvokeSysExCallback(_MessageBuffer, _MessageSize)) {
                    InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                }

                _Status = Status::Idle;
                _MessageSize = 0;
            }
        }

}
